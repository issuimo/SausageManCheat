#include <pch.h>
#include "Patch.h"

#include <Base/util.h>

#include <utility>

bool Patch::Install(uint64_t address, std::vector<uint8_t> value)
{
	if (patches.contains(address))
	{
		LOG_ERROR("安装补丁失败: 已安装补丁.");
		return false;
	}

	const auto oldValue = WriteMemory(address, std::move(value));
	if (oldValue == nullptr)
		return false;

	patches[address] = oldValue;
	return true;
}

bool Patch::Restore(uint64_t address)
{
	if (!patches.contains(address))
	{
		LOG_ERROR("修复补丁失败: 没有找到补丁到目标地址 0x%016X", address);
		return false;
	}

	auto restoreValue = patches[address];
	auto oldValue = WriteMemory(address, *restoreValue);
	if (oldValue == nullptr)
		return false;

	patches.erase(address);
	delete restoreValue;
	delete oldValue;

	return true;
}

std::vector<uint8_t>* Patch::WriteMemory(uint64_t address, const std::vector<uint8_t>& value) 
{
	MEMORY_BASIC_INFORMATION information{};
	auto size = VirtualQuery(reinterpret_cast<void*>(address), &information, sizeof(information));
	if (size < sizeof(information))
	{
		LOG_LAST_ERROR("获取页面信息失败");
		return nullptr;
	}

	if (information.State != MEM_COMMIT) 
	{
		LOG_ERROR("目标地址处的页不是 MEM_COMMIT (0x%016X)", address);
		return nullptr;
	}
		

	DWORD oldProtection = -1;
	if ((information.AllocationProtect & PAGE_READWRITE) == 0 && (information.AllocationProtect & PAGE_EXECUTE_READWRITE) == 0)
	{
		if (VirtualProtect(reinterpret_cast<void*>(address), value.size(), PAGE_EXECUTE_READWRITE, &oldProtection) == FALSE)
		{
			LOG_LAST_ERROR("更改页面保护失败");
			return nullptr;
		}
	}

	auto oldValue = new std::vector<uint8_t>(value.size());
	auto errorCode = memcpy_s(oldValue->data(), value.size(), reinterpret_cast<void*>(address), value.size());
	if (errorCode != 0)
	{
		LOG_ERROR("从内存中获取原始值失败 0x%016X. 错误代码: %d", address, errorCode);
		delete oldValue;
		return nullptr;
	}

	errorCode = memcpy_s(reinterpret_cast<void*>(address), value.size(), value.data(), value.size());
	if (errorCode != 0)
	{
		LOG_ERROR("未能重写目标内存 0x%016X. 错误代码: %d", address, errorCode);
		delete oldValue;
		return nullptr;
	}

	if (oldProtection != -1)
	{
		DWORD temp = 0;
		if (VirtualProtect(reinterpret_cast<void*>(address), value.size(), oldProtection, &temp) == FALSE)
		{
			LOG_LAST_ERROR("恢复页面保护失败");
		}
	}
	return oldValue;
}
