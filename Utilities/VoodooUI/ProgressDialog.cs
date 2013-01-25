/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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
    public struct StagedCommand
    {
        public ProgressDialog.StageDelegate Pre, Post;
        public String Command;

        public StagedCommand(ProgressDialog.StageDelegate pre, String cmd, ProgressDialog.StageDelegate post)
        {
            Pre = pre;
            Post = post;
            Command = cmd;
        }
    };

    public partial class ProgressDialog : Form
    {
        BackgroundWorker m_Worker;

        public delegate void StageDelegate();
        public Queue<StagedCommand> CommandQueue { get; set; }

        public void QueueCommand(String command)
        {
            CommandQueue.Enqueue(new StagedCommand(null, command, null));
        }

        public void QueueCommand(StageDelegate pre, String cmd, StageDelegate post)
        {
            CommandQueue.Enqueue(new StagedCommand(pre, cmd, post));
        }

        public ProgressDialog()
        {
            InitializeComponent();
            CommandQueue = new Queue<StagedCommand>();
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
            }
            else
            {
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

        void m_Worker_DoWork(object sender, DoWorkEventArgs e)
        {
            int exec = 1;
            while (CommandQueue != null && CommandQueue.Count > 0)
            {
                if (e.Cancel)
                {
                    e.Result = 1;
                    return;
                }

                WriteLine("Running command {0}, {1} remaining.", exec++, CommandQueue.Count);
                StagedCommand command = CommandQueue.Peek();
                try
                {
                    if (command.Pre != null) command.Pre();
                    int result = ShellExec(command.Command);
                    if (command.Post != null) command.Post();

                    if (result != 0)
                    {
                        e.Result = result;
                        return;
                    }
                    else
                    {
                        CommandQueue.Dequeue();
                    }
                }
                catch (Exception exc)
                {
                    WriteLine("Error: {0}", exc.Message);
                    e.Result = 1;
                    return;
                }
            }

            WriteLine("All commands executed successfully.");
            e.Result = 0;
            return;
        }

        int ShellExec(String command)
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

            Process shell = new Process();
            shell.StartInfo = info;
            shell.EnableRaisingEvents = true;
            shell.ErrorDataReceived += new DataReceivedEventHandler(ShellErrorDataReceived);
            shell.OutputDataReceived += new DataReceivedEventHandler(ShellOutputDataReceived);

            shell.Start();
            shell.BeginErrorReadLine();
            shell.BeginOutputReadLine();
            shell.WaitForExit();

            return shell.ExitCode;
        }

        void m_Worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            int exitCode = (int)e.Result;
                
            if (e.Cancelled)
            {
                WriteLine("Command canceled.");
            }
            else if (exitCode != 0)
            {
                WriteLine("Command returned exit code {0}.", exitCode);
            }
            else
            {
                WriteLine("Command returned successful exit code.");
            }

            SetAllowClose(true);
            SetProgressVisible(false);
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
            if (m_Worker != null)
            {
                m_Worker.CancelAsync();
            }

            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }

        private void OnShow(object sender, EventArgs e)
        {
            if (CommandQueue != null && CommandQueue.Count > 0)
            {
                m_Worker = new BackgroundWorker();
                m_Worker.WorkerReportsProgress = true;
                m_Worker.WorkerSupportsCancellation = true;
                m_Worker.DoWork += new DoWorkEventHandler(m_Worker_DoWork);
                m_Worker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(m_Worker_RunWorkerCompleted);

                m_Worker.RunWorkerAsync();
            }
        }
    }
}
