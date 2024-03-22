namespace InjectHelper ; 
    public partial class Form1 : Form {
        private MySql.Data.MySqlClient.MySqlConnection conn;

        public Form1() {
            if (MachineCode.GetMachineCodeString().IndexOf("Virtual") != -1) Environment.Exit(0);
            this.InitializeComponent();
        }

        [System.Runtime.InteropServices.DllImportAttribute("./GPP32.dll")]
        private static extern void Inject();

        private void button1_Click(object sender, EventArgs e_) {
            Form1.Inject();
            new Thread(() => {
                Thread.Sleep(3000);
                Environment.Exit(0);
            }).Start();
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e) { MessageBox.Show("自己浏览器手动输入网址!", "嘿嘿", MessageBoxButtons.OK); }

        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e) { MessageBox.Show("自己浏览器手动输入网址!", "嘿嘿", MessageBoxButtons.OK); }

        private void Form1_Load(object sender, EventArgs e) {
            this.conn = new MySql.Data.MySqlClient.MySqlConnection("server = suimo-db-main.mysql.polardb.rds.aliyuncs.com; user = xc_user; database = xc; port = 4328; password = 114514;");

            try {
                this.conn.Open();

                using (var command = new MySql.Data.MySqlClient.MySqlCommand("SELECT * FROM rmb", this.conn)) {
                    using (MySql.Data.MySqlClient.MySqlDataReader reader = command.ExecuteReader()) {
                        while (reader.HasRows && reader.Read()) {
                            string name = reader.GetString("name");
                            this.listBox1.Items.Add(name);
                        }
                        reader.Close();
                    }
                }

                this.textBox1.Text = MachineCode.GetMachineCodeString();

                this.conn.Close();
            } catch (Exception ex) {
                MessageBox.Show("数据库操作失败!\n" + ex, "错误", MessageBoxButtons.OK);
            }
        }
    }