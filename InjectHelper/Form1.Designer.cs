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
            this.linkLabel1 = new LinkLabel();
            this.label2 = new Label();
            this.button1 = new Button();
            this.label3 = new Label();
            this.textBox1 = new TextBox();
            this.groupBox1 = new GroupBox();
            this.label4 = new Label();
            SuspendLayout();
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
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new Point(53, 36);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new Size(60, 17);
            this.linkLabel1.TabIndex = 1;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "遂沫.com";
            this.linkLabel1.LinkClicked += linkLabel1_LinkClicked;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new Point(12, 36);
            this.label2.Name = "label2";
            this.label2.Size = new Size(35, 17);
            this.label2.TabIndex = 2;
            this.label2.Text = "网址:";
            // 
            // button1
            // 
            this.button1.Location = new Point(12, 225);
            this.button1.Name = "button1";
            this.button1.Size = new Size(168, 52);
            this.button1.TabIndex = 3;
            this.button1.Text = "线程劫持注入\r\n";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += button1_Click;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new Point(13, 176);
            this.label3.Name = "label3";
            this.label3.Size = new Size(47, 17);
            this.label3.TabIndex = 4;
            this.label3.Text = "邀请码:";
            // 
            // textBox1
            // 
            this.textBox1.Location = new Point(13, 196);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new Size(167, 23);
            this.textBox1.TabIndex = 5;
            // 
            // groupBox1
            // 
            this.groupBox1.Location = new Point(186, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new Size(333, 265);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "公告";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new Font("Microsoft YaHei UI", 12F, FontStyle.Regular, GraphicsUnit.Point, 134);
            this.label4.ForeColor = Color.Red;
            this.label4.Location = new Point(13, 65);
            this.label4.Name = "label4";
            this.label4.Size = new Size(151, 105);
            this.label4.TabIndex = 7;
            this.label4.Text = "1.本产品是免费辅助\r\n2.邀请码也是免费的\r\n3.邀请码请进群获取\r\n4.邀请码为一码一机\r\n5.打开游戏后再注入\r\n";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 17F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(531, 289);
            Controls.Add(this.label4);
            Controls.Add(this.groupBox1);
            Controls.Add(this.textBox1);
            Controls.Add(this.label3);
            Controls.Add(this.button1);
            Controls.Add(this.label2);
            Controls.Add(this.linkLabel1);
            Controls.Add(this.label1);
            MaximumSize = new Size(547, 328);
            MinimumSize = new Size(547, 328);
            Name = "Form1";
            Text = "Inject";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label label1;
        private LinkLabel linkLabel1;
        private Label label2;
        private Button button1;
        private Label label3;
        private TextBox textBox1;
        private GroupBox groupBox1;
        private Label label4;
    }
}
