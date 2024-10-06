namespace Tank_game
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Send_Btn = new System.Windows.Forms.Button();
            this.Peticion_txtBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // Send_Btn
            // 
            this.Send_Btn.Location = new System.Drawing.Point(79, 108);
            this.Send_Btn.Name = "Send_Btn";
            this.Send_Btn.Size = new System.Drawing.Size(75, 23);
            this.Send_Btn.TabIndex = 0;
            this.Send_Btn.Text = "Send";
            this.Send_Btn.UseVisualStyleBackColor = true;
            this.Send_Btn.Click += new System.EventHandler(this.Send_Btn_Click);
            // 
            // Peticion_txtBox
            // 
            this.Peticion_txtBox.Location = new System.Drawing.Point(68, 56);
            this.Peticion_txtBox.Name = "Peticion_txtBox";
            this.Peticion_txtBox.Size = new System.Drawing.Size(100, 20);
            this.Peticion_txtBox.TabIndex = 1;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.Peticion_txtBox);
            this.Controls.Add(this.Send_Btn);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Send_Btn;
        private System.Windows.Forms.TextBox Peticion_txtBox;
    }
}

