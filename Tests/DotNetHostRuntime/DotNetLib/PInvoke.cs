using System.Runtime.InteropServices;

public partial class CSharpLib
{
    // Import user32.dll (containing the function we need) and define
    // the method corresponding to the native function.
    [LibraryImport("User32.dll", EntryPoint = "MessageBoxW", StringMarshalling =
        StringMarshalling.Utf16, SetLastError = true)]
    private static partial int MessageBox(IntPtr hWnd, string lpText, string lpCaption, uint uType);

    public static void Main(string[] args)
    {
        // Invoke the function as a regular managed method.
        MessageBox(IntPtr.Zero, "Command-line message box", "Attention!", 0);
    }
}
