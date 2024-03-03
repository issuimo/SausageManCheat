namespace InjectHelper ; 
    public partial class Form1 : Form {
        private MySql.Data.MySqlClient.MySqlConnection conn;

        public Form1() {
            if (MachineCode.GetMachineCodeString().IndexOf(value: "Virtual") != -1) Environment.Exit(exitCode: 0);
            this.InitializeComponent();
        }

        [System.Runtime.InteropServices.DllImportAttribute(dllName: "./GPP32.dll")] private static extern void Inject();

        private void button1_Click(object sender, EventArgs e_) {
            string txt = this.textBox1.Text;
            if (this.textBox1.Text.Equals(value: "")) {
                MessageBox.Show(text: "请输入邀请码!", caption: "错误", buttons: MessageBoxButtons.OK);
                return;
            }
            try {
                this.conn.Open();
                using (var command = new MySql.Data.MySqlClient.MySqlCommand(cmdText: $"SELECT * FROM users WHERE `key`='{txt}'", connection: this.conn)) {
                    using (MySql.Data.MySqlClient.MySqlDataReader reader = command.ExecuteReader()) {
                        if (reader.HasRows && reader.Read()) {
                            string code = reader.GetString(column: "code");
                            reader.Close();
                            if (code.Equals(value: "null")) {
                                using (var command2 = new MySql.Data.MySqlClient.MySqlCommand(cmdText: $"SELECT * FROM users WHERE code='{MachineCode.GetMachineCodeString()}'", connection: this.conn)) {
                                    using (MySql.Data.MySqlClient.MySqlDataReader reader2 = command2.ExecuteReader()) {
                                        if (reader2.HasRows && reader2.Read()) {
                                            string code2 = reader2.GetString(i: 0);
                                            MessageBox.Show(text: $"你已拥有邀请码，请使用自己的邀请码: {code2}!", caption: "错误", buttons: MessageBoxButtons.OK);
                                            reader2.Close();
                                            return;
                                        }
                                    }
                                }

                                using (var command2 = new MySql.Data.MySqlClient.MySqlCommand(cmdText: $"UPDATE users SET code='{MachineCode.GetMachineCodeString()}' WHERE `key`='{txt}'", connection: this.conn)) {
                                    int rowsAffected = command2.ExecuteNonQuery();
                                    if (rowsAffected > 0) {
                                        MessageBox.Show(text: "激活成功!", caption: "成功", buttons: MessageBoxButtons.OK);
                                        code = MachineCode.GetMachineCodeString();
                                    } else MessageBox.Show(text: "激活失败!", caption: "错误", buttons: MessageBoxButtons.OK);
                                }
                            }
                            if (MachineCode.GetMachineCodeString().Equals(value: code)) {
                                this.conn.Close();
                                File.WriteAllText(path: @"C:\key.dat", contents: $"{MachineCode.GetMachineCodeString()}\n{txt}");
                                Form1.Inject();
                                new Thread(start: () => {
                                    Thread.Sleep(millisecondsTimeout: 3000);
                                    Environment.Exit(exitCode: 0);
                                }).Start();
                            } else if (code.Equals(value: "ban")) MessageBox.Show(text: "该邀请码已被封禁!", caption: "错误", buttons: MessageBoxButtons.OK);
                            else MessageBox.Show(text: "该邀请码已被激活且不属于你!", caption: "错误", buttons: MessageBoxButtons.OK);
                        } else MessageBox.Show(text: "无效邀请码!", caption: "错误", buttons: MessageBoxButtons.OK);
                    }
                }

                this.conn.Close();
            } catch (Exception ex) {
                MessageBox.Show(text: "数据库操作失败!\n" + ex, caption: "错误", buttons: MessageBoxButtons.OK);
            }
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e) { MessageBox.Show(text: "自己浏览器手动输入网址!", caption: "嘿嘿", buttons: MessageBoxButtons.OK); }

        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e) { MessageBox.Show(text: "自己浏览器手动输入网址!", caption: "嘿嘿", buttons: MessageBoxButtons.OK); }

        private void Form1_Load(object sender, EventArgs e) {
            this.conn = new MySql.Data.MySqlClient.MySqlConnection(connectionString: "server = suimo-db-main.mysql.polardb.rds.aliyuncs.com; user = xc_user; database = xc; port = 4328; password = 114514;");

            try {
                this.conn.Open();

                using (var command = new MySql.Data.MySqlClient.MySqlCommand(cmdText: $"SELECT * FROM ban WHERE code='{MachineCode.GetMachineCodeString()}'", connection: this.conn)) {
                    using (MySql.Data.MySqlClient.MySqlDataReader reader = command.ExecuteReader()) {
                        if (reader.HasRows && reader.Read()) {
                            string code = reader.GetString(column: "code");
                            if (MachineCode.GetMachineCodeString().Equals(value: code)) {
                                this.conn.Close();
                                new Thread(start: () => {
                                    Thread.Sleep(millisecondsTimeout: 3000);
                                    Environment.Exit(exitCode: 0);
                                }).Start();
                                MessageBox.Show(text: "该机器已被封禁!", caption: "错误", buttons: MessageBoxButtons.OK);
                            }
                        }
                        reader.Close();
                    }
                }

                using (var command = new MySql.Data.MySqlClient.MySqlCommand(cmdText: "SELECT * FROM rmb", connection: this.conn)) {
                    using (MySql.Data.MySqlClient.MySqlDataReader reader = command.ExecuteReader()) {
                        while (reader.HasRows && reader.Read()) {
                            string name = reader.GetString(column: "name");
                            this.listBox1.Items.Add(item: name);
                        }
                        reader.Close();
                    }
                }

                using (var command = new MySql.Data.MySqlClient.MySqlCommand(cmdText: "SELECT * FROM info", connection: this.conn)) {
                    using (MySql.Data.MySqlClient.MySqlDataReader reader = command.ExecuteReader()) {
                        while (reader.HasRows && reader.Read()) {
                            string name = reader.GetString(column: "name");
                            string doc = reader.GetString(column: "doc");
                            this.listBox2.Items.Add(item: name + ":" + doc);
                        }
                        reader.Close();
                    }
                }

                using (var command2 = new MySql.Data.MySqlClient.MySqlCommand(cmdText: $"SELECT * FROM users WHERE code='{MachineCode.GetMachineCodeString()}'", connection: this.conn)) {
                    using (MySql.Data.MySqlClient.MySqlDataReader reader2 = command2.ExecuteReader()) {
                        if (reader2.HasRows && reader2.Read()) {
                            string code2 = reader2.GetString(i: 0);
                            this.textBox1.Text = code2;
                            reader2.Close();
                        }
                    }
                }

                this.conn.Close();
            } catch (Exception ex) {
                MessageBox.Show(text: "数据库操作失败!\n" + ex, caption: "错误", buttons: MessageBoxButtons.OK);
            }
        }
    }