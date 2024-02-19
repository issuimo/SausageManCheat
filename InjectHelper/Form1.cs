using MySql.Data;
using System.Data;
using MySql.Data.MySqlClient;
using Microsoft.VisualBasic.ApplicationServices;
using System.Dynamic;
using System.Runtime.InteropServices;
using static System.Net.Mime.MediaTypeNames;

namespace InjectHelper {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();
        }

        [DllImport("./GPP32.dll")]
        private extern static void Inject();

        private void button1_Click(object sender, EventArgs e_) {
            string txt = textBox1.Text;
            if (textBox1.Text.Equals("")) {
                MessageBox.Show("请输入邀请码!", "错误", MessageBoxButtons.OK);
                return;
            }
            string sb = "失败";
            try {
                int port = 4328;
                int pwd = 114514;
                pwd += 10;
                port += pwd;
                string xc = "x" + "c";
                string r = "r";
                string s = "s";
                string o = "o";
                string u = "u";
                string i = "i";
                string m = "m";
                string e = "e";
                string xc_user = xc + "_" + u + s + e + r;
                string port2 = "p" + o + r + "t";
                string suimo = s + u + i + m + o;
                string user = u + s + e + r;
                string yqm = "邀请码";
                string jh = "激活";
                using (MySqlConnection conn = new MySqlConnection($"server = {suimo}-db-main.mysql.polardb.rds.aliyuncs.com; {user} = {xc_user}; database = {xc}; {port2} = {port - pwd}; password = {pwd - 10};")) {
                    conn.Open();

                    using (MySqlCommand command = new MySqlCommand($"SELECT * FROM {user}s WHERE `key`='{txt}'", conn)) {
                        using (MySqlDataReader reader = command.ExecuteReader()) {
                            if (reader.HasRows && reader.Read()) {
                                string code = reader.GetString("code");
                                reader.Close();
                                if (code.Equals("null")) {
                                    using (MySqlCommand command2 = new MySqlCommand($"UPDATE {user}s SET code='{MachineCode.GetMachineCodeString()}' WHERE `key`='{txt}'", conn)) {
                                        int rowsAffected = command2.ExecuteNonQuery();
                                        if (rowsAffected > 0) {
                                            MessageBox.Show($"{jh}成功!", "成功", MessageBoxButtons.OK);
                                            code = MachineCode.GetMachineCodeString();
                                        } else {
                                            MessageBox.Show($"{jh}{sb}!", "错误", MessageBoxButtons.OK);
                                        }
                                    }
                                } 
                                if (MachineCode.GetMachineCodeString().Equals(code)) {
                                    conn.Close();
                                    File.WriteAllText(@"C:\key.dat", $"{MachineCode.GetMachineCodeString()}\n{txt}");
                                    Inject();
                                    Environment.Exit(0);
                                } else {
                                    MessageBox.Show($"该已{yqm}被{jh}且不属于你!", "错误", MessageBoxButtons.OK);
                                }
                            } else {
                                MessageBox.Show($"无效{yqm}!", "错误", MessageBoxButtons.OK);
                            }
                        }
                    }

                    conn.Close();
                }
            } catch (Exception ex) {
                MessageBox.Show($"数据库操作{sb}!\n" + ex.ToString(), "错误", MessageBoxButtons.OK);
            }
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e) {
            MessageBox.Show("自己浏览器手动输入网址!", "嘿嘿", MessageBoxButtons.OK);
        }
    }
}
