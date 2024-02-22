using MySql.Data;
using System.Data;
using MySql.Data.MySqlClient;
using Microsoft.VisualBasic.ApplicationServices;
using System.Dynamic;
using System.Runtime.InteropServices;
using static System.Net.Mime.MediaTypeNames;

namespace InjectHelper {
    public partial class Form1 : Form { 

        private MySqlConnection conn;

        public Form1() {
            if (MachineCode.GetMachineCodeString().IndexOf("Virtual") != -1) {
                Environment.Exit(0);
            }
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
            try {
                
                conn.Open();
                using (MySqlCommand command = new MySqlCommand($"SELECT * FROM users WHERE `key`='{txt}'", conn)) {
                        using (MySqlDataReader reader = command.ExecuteReader()) {
                            if (reader.HasRows && reader.Read()) {
                                string code = reader.GetString("code");
                                reader.Close();
                                if (code.Equals("null")) {
                                    using (MySqlCommand command2 = new MySqlCommand($"SELECT * FROM users WHERE code='{MachineCode.GetMachineCodeString()}'", conn)) {
                                         using (MySqlDataReader reader2 = command2.ExecuteReader()) {
                                            if (reader2.HasRows && reader2.Read()) {
                                                string code2 = reader2.GetString(0);
                                                MessageBox.Show($"你已拥有邀请码，请使用自己的邀请码: {code2}!", "错误", MessageBoxButtons.OK);
                                                reader2.Close();
                                                return;
                                            }
                                         }
                                    }

                                    using (MySqlCommand command2 = new MySqlCommand($"UPDATE users SET code='{MachineCode.GetMachineCodeString()}' WHERE `key`='{txt}'", conn)) {
                                        int rowsAffected = command2.ExecuteNonQuery();
                                        if (rowsAffected > 0) {
                                            MessageBox.Show($"激活成功!", "成功", MessageBoxButtons.OK);
                                            code = MachineCode.GetMachineCodeString();
                                        } else {
                                            MessageBox.Show($"激活失败!", "错误", MessageBoxButtons.OK);
                                        }
                                    }
                                }
                                if (MachineCode.GetMachineCodeString().Equals(code)) {
                                    conn.Close();
                                    File.WriteAllText(@"C:\key.dat", $"{MachineCode.GetMachineCodeString()}\n{txt}");
                                    Inject();
                                    new Thread(()=>{ 
                                    Thread.Sleep(3000);
                                    Environment.Exit(0);
                                }).Start();
                                } else if (code.Equals("ban")) { 
                                    MessageBox.Show($"该邀请码已被封禁!", "错误", MessageBoxButtons.OK);
                                } else {
                                    MessageBox.Show($"该邀请码已被激活且不属于你!", "错误", MessageBoxButtons.OK);
                                }
                            } else {
                                MessageBox.Show($"无效邀请码!", "错误", MessageBoxButtons.OK);
                            }
                        }
                 }

                conn.Close();
            } catch (Exception ex) {
                MessageBox.Show($"数据库操作失败!\n" + ex.ToString(), "错误", MessageBoxButtons.OK);
            }
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e) {
            MessageBox.Show("自己浏览器手动输入网址!", "嘿嘿", MessageBoxButtons.OK);
        }

        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e) {
            MessageBox.Show("自己浏览器手动输入网址!", "嘿嘿", MessageBoxButtons.OK);
        }

        private void Form1_Load(object sender, EventArgs e) {
            conn = new MySqlConnection($"server = suimo-db-main.mysql.polardb.rds.aliyuncs.com; user = xc_user; database = xc; port = 4328; password = 114514;");

            try {
                conn.Open();

                using (MySqlCommand command = new MySqlCommand($"SELECT * FROM ban WHERE code='{MachineCode.GetMachineCodeString()}'", conn)) {
                    using (MySqlDataReader reader = command.ExecuteReader()) {
                         if (reader.HasRows && reader.Read()) {
                            string code = reader.GetString("code");
                            if (MachineCode.GetMachineCodeString().Equals(code)) {
                                conn.Close();
                                new Thread(()=>{ 
                                    Thread.Sleep(3000);
                                    Environment.Exit(0);
                                }).Start();
                                MessageBox.Show($"该机器已被封禁!", "错误", MessageBoxButtons.OK);
                            }
                        }
                        reader.Close();
                    }
                 }

                 using (MySqlCommand command = new MySqlCommand($"SELECT * FROM rmb", conn)) {
                    using (MySqlDataReader reader = command.ExecuteReader()) {
                        while (reader.HasRows && reader.Read()) {
                            string name = reader.GetString("name");
                            listBox1.Items.Add(name);
                        }
                        reader.Close();
                    }
                 }

                 using (MySqlCommand command = new MySqlCommand($"SELECT * FROM info", conn)) {
                    using (MySqlDataReader reader = command.ExecuteReader()) {
                        while (reader.HasRows && reader.Read()) {
                            string name = reader.GetString("name");
                            string doc = reader.GetString("doc");
                            listBox2.Items.Add(name + ":" + doc);
                        }
                        reader.Close();
                    }
                 }

                 using (MySqlCommand command2 = new MySqlCommand($"SELECT * FROM users WHERE code='{MachineCode.GetMachineCodeString()}'", conn)) {
                                         using (MySqlDataReader reader2 = command2.ExecuteReader()) {
                                            if (reader2.HasRows && reader2.Read()) {
                                                string code2 = reader2.GetString(0);
                                                textBox1.Text = code2;
                                                reader2.Close();
                        }
                                         }
                                    }

                conn.Close();
            } catch (Exception ex) {
                MessageBox.Show($"数据库操作失败!\n" + ex.ToString(), "错误", MessageBoxButtons.OK);
            }
        }
    }
}
