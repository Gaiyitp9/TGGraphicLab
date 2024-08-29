using System.Runtime.InteropServices;

namespace PInvokeExample
{
    public partial class App
    {
        private static byte sIsWaiting = 0;
        private static int sCallCount = 0;
        private static ManualResetEvent sMRE = new ManualResetEvent(false);
        
        // Import user32.dll (containing the function we need) and define
        // the method corresponding to the native function.
        [LibraryImport("User32.dll", EntryPoint = "MessageBoxW", StringMarshalling =
            StringMarshalling.Utf16, SetLastError = true)]
        private static partial int MessageBox(IntPtr hWnd, string lpText, string lpCaption, uint uType);

        public static void Main(string[] args)
        {
            Console.WriteLine($"{nameof(App)} started - args = [ {string.Join(", ", args)} ]");
            sIsWaiting = 1;
            sMRE.WaitOne();
            // Invoke the function as a regular managed method.
            MessageBox(IntPtr.Zero, "Command-line message box", "Attention!", 0);
        }

        [UnmanagedCallersOnly]
        public static byte IsWaiting() => sIsWaiting;

        [UnmanagedCallersOnly]
        public static void Hello(IntPtr message)
        {
            Console.WriteLine($"Hello, world! from {nameof(App)} [count: {++sCallCount}]");
            Console.WriteLine($"-- message: {Marshal.PtrToStringUTF8(message)}");
            if (sCallCount >= 3)
            {
                Console.WriteLine("Signaling app to close");
                sMRE.Set();
            }
        }
    }
}
