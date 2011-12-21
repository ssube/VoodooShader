/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace VoodooUI
{
    public partial class ProgressDialog : Form
    {
        Process m_ShellProcess;
        object m_Lock = new object();

        public ProgressDialog()
        {
            InitializeComponent();
        }

        delegate void ClearDelegate();
        public void Clear()
        {
            if (cDetails.InvokeRequired)
            {
                ClearDelegate d = new ClearDelegate(Clear);
                this.Invoke(d);
            }
            else
            {
                cDetails.Text = String.Empty;
            }
        }

        delegate void WriteDelegate(String msg, params object[] args);
        public void WriteLine(String msg, params object[] args)
        {
            if (!String.IsNullOrEmpty(msg))
            {
                if (cDetails.InvokeRequired)
                {
                    WriteDelegate d = new WriteDelegate(WriteLine);
                    this.Invoke(d, msg, args);
                }
                else
                {
                    cDetails.Text += String.Format(msg, args) + Environment.NewLine;
                }
            }
        }

        public void Write(String msg, params object[] args)
        {
            if (!String.IsNullOrEmpty(msg))
            {
                if (cDetails.InvokeRequired)
                {
                    WriteDelegate d = new WriteDelegate(Write);
                    this.Invoke(new WriteDelegate(Write), msg, args);
                }
                else
                {
                    cDetails.Text += String.Format(msg, args);
                }
            }
        }

        delegate void AllowCloseDelegate(bool mode);
        void SetAllowClose(bool mode)
        {
            if (bOK.InvokeRequired)
            {
                AllowCloseDelegate d = new AllowCloseDelegate(SetAllowClose);
                this.Invoke(d, mode);
            } else {
                bOK.Enabled = mode;
            }
        }

        public bool AllowClose
        {
            get { return bOK.Enabled; }
            set { SetAllowClose(value); }
        }

        delegate void ProgressDelegate(int percent);
        void SetProgress(int percent)
        {
            if (cProgressBar.InvokeRequired)
            {
                ProgressDelegate d = new ProgressDelegate(SetProgress);
                this.Invoke(d, percent);
            }
            else
            {
                cProgressBar.Value = percent;
            }
        }

        public int Progress
        {
            get { return cProgressBar.Value; }
            set { SetProgress(value); }
        }

        delegate void StyleDelegate(ProgressBarStyle style);
        void SetStyle(ProgressBarStyle style)
        {
            if (cProgressBar.InvokeRequired)
            {
                StyleDelegate d = new StyleDelegate(SetStyle);
                this.Invoke(d, style);
            }
            else
            {
                cProgressBar.Style = style;
            }
        }

        public ProgressBarStyle Style
        {
            get { return cProgressBar.Style; }
            set { SetStyle(value); }
        }

        delegate void ProgressVisibleDelegate(bool visible);
        void SetProgressVisible(bool visible)
        {
            if (cProgressBar.InvokeRequired)
            {
                ProgressVisibleDelegate d = new ProgressVisibleDelegate(SetProgressVisible);
                this.Invoke(d, visible);
            }
            else
            {
                cProgressBar.Visible = visible;
            }
        }

        public bool ProgressVisible
        {
            get { return cProgressBar.Visible; }
            set { SetProgressVisible(value); }
        }

        public DialogResult ShellExecute(string command)
        {
            lock (m_Lock)
            {
                WriteLine(command);
                Style = ProgressBarStyle.Marquee;

                ProcessStartInfo info = new ProcessStartInfo
                {
                    UseShellExecute = false,
                    LoadUserProfile = true,
                    ErrorDialog = false,
                    CreateNoWindow = true,
                    WindowStyle = ProcessWindowStyle.Hidden,
                    RedirectStandardOutput = true,
                    StandardOutputEncoding = Encoding.UTF8,
                    RedirectStandardError = true,
                    StandardErrorEncoding = Encoding.UTF8,

                    FileName = "cmd.exe",
                    Arguments = "/c " + command
                };

                m_ShellProcess = new Process();
                m_ShellProcess.StartInfo = info;
                m_ShellProcess.EnableRaisingEvents = true;
                m_ShellProcess.ErrorDataReceived += new DataReceivedEventHandler(ShellErrorDataReceived);
                m_ShellProcess.OutputDataReceived += new DataReceivedEventHandler(ShellOutputDataReceived);
                m_ShellProcess.Exited += new EventHandler(ShellExited);

                m_ShellProcess.Start();
                m_ShellProcess.BeginErrorReadLine();
                m_ShellProcess.BeginOutputReadLine();

                while (m_ShellProcess != null && !m_ShellProcess.HasExited)
                {
                    Application.DoEvents();
                    Thread.Sleep(100);
                }

                return DialogResult.OK;
            }
        }

        void ShellExited(object sender, EventArgs e)
        {
            SetAllowClose(true);
            SetProgressVisible(false);
            m_ShellProcess = null;
        }

        void ShellOutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (!String.IsNullOrEmpty(e.Data as String))
            {
                WriteLine("{0}", e.Data);
            }
        }

        void ShellErrorDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (!String.IsNullOrEmpty(e.Data as String))
            {
                WriteLine("{0}", e.Data);
            }
        }

        private void ButtonOK(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void ButtonCancel(object sender, EventArgs e)
        {
            if (m_ShellProcess != null)
            {
                m_ShellProcess.Kill();
            }

            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
