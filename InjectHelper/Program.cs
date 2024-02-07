namespace InjectHelper {
    internal static class Program {
        [System.Runtime.InteropServices.DllImportAttribute("GPP32.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Cdecl)]
        public extern static void Inject();

        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main() {
            // To customize application configuration such as set high DPI settings or default font,
            // see https://aka.ms/applicationconfiguration.
            ApplicationConfiguration.Initialize();
            Application.Run(new Form1());
        }
    }
}