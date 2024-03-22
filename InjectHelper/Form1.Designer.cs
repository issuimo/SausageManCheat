namespace InjectHelper {
    partial class Form1 {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.label1 = new Label();
            this.button1 = new Button();
            this.groupBox2 = new GroupBox();
            this.listBox1 = new ListBox();
            this.label3 = new Label();
            this.textBox1 = new TextBox();
            this.label4 = new Label();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new Size(122, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "QQ群聊: 472659840";
            // 
            // button1
            // 
            this.button1.Location = new Point(12, 225);
            this.button1.Name = "button1";
            this.button1.Size = new Size(168, 40);
            this.button1.TabIndex = 3;
            this.button1.Text = "线程劫持注入\r\n";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += this.button1_Click;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.listBox1);
            this.groupBox2.Location = new Point(194, 22);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new Size(167, 243);
            this.groupBox2.TabIndex = 15;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "赞助名单 (排名不分前后)";
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 17;
            this.listBox1.Location = new Point(6, 22);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new Size(155, 208);
            this.listBox1.TabIndex = 0;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new Point(13, 176);
            this.label3.Name = "label3";
            this.label3.Size = new Size(47, 17);
            this.label3.TabIndex = 4;
            this.label3.Text = "机器码:";
            // 
            // textBox1
            // 
            this.textBox1.Location = new Point(13, 196);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new Size(167, 23);
            this.textBox1.TabIndex = 5;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new Font("Microsoft YaHei UI Light", 10.5F, FontStyle.Regular, GraphicsUnit.Point, 134);
            this.label4.ForeColor = Color.Red;
            this.label4.Location = new Point(13, 34);
            this.label4.Name = "label4";
            this.label4.Size = new Size(132, 120);
            this.label4.TabIndex = 7;
            this.label4.Text = "1.本产品是免费辅助\r\n2.打开游戏后再注入\r\n3.打开游戏后再注入\r\n4.打开游戏后再注入\r\n5.打开游戏后再注入\r\n6.打开游戏后再注入";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new SizeF(7F, 17F);
            this.AutoScaleMode = AutoScaleMode.Font;
            this.ClientSize = new Size(369, 272);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.MaximumSize = new Size(385, 311);
            this.MinimumSize = new Size(385, 311);
            this.Name = "Form1";
            this.Text = "Inject";
            this.Load += this.Form1_Load;
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        #endregion

        private Label label1;
        private Button button1;
        private GroupBox groupBox2;
        private ListBox listBox1;
        private Label label3;
        private TextBox textBox1;
        private Label label4;
    }
}
