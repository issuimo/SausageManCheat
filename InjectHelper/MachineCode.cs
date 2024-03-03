namespace InjectHelper ; 
    internal class MachineCode {
        private static MachineCode machineCode;

        public static string GetMachineCodeString() {
            var machineCodeString = string.Empty;
            if (MachineCode.machineCode == null) MachineCode.machineCode = new MachineCode();
            machineCodeString = "PC." + MachineCode.machineCode.GetCpuInfo() + "." + MachineCode.machineCode.GetHDid();
            return machineCodeString;
        }

        /// <summary>
        ///     获取cpu序列号
        /// </summary>
        /// <returns> string </returns>
        public string GetCpuInfo() {
            var cpuInfo = "";
            using (var cimobject = new System.Management.ManagementClass("Win32_Processor")) {
                System.Management.ManagementObjectCollection moc = cimobject.GetInstances();

                foreach (System.Management.ManagementObject mo in moc) {
                    cpuInfo = mo.Properties["ProcessorId"].Value.ToString();
                    mo.Dispose();
                }
            }
            return cpuInfo;
        }

        /// <summary>
        ///     获取硬盘ID
        /// </summary>
        /// <returns> string </returns>
        public string GetHDid() {
            var HDid = "";
            using (var cimobject1 = new System.Management.ManagementClass("Win32_DiskDrive")) {
                System.Management.ManagementObjectCollection moc1 = cimobject1.GetInstances();
                foreach (System.Management.ManagementObject mo in moc1) {
                    HDid = (string)mo.Properties["Model"].Value;
                    mo.Dispose();
                }
            }
            return HDid;
        }

        /// <summary>
        ///     获取网卡硬件地址
        /// </summary>
        /// <returns> string </returns>
        public string GetMoAddress() {
            var MoAddress = "";
            using (var mc = new System.Management.ManagementClass("Win32_NetworkAdapterConfiguration")) {
                System.Management.ManagementObjectCollection moc2 = mc.GetInstances();
                foreach (System.Management.ManagementObject mo in moc2) {
                    if ((bool)mo["IPEnabled"]) MoAddress = mo["MacAddress"].ToString();
                    mo.Dispose();
                }
            }
            return MoAddress;
        }
    }