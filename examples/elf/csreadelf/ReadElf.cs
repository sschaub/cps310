// ReadElf.cs
// Demonstrates reading an ELF format file

using System;
using System.Runtime.InteropServices;
using System.IO;


// A struct that mimics memory layout of ELF file header
// See http://www.sco.com/developers/gabi/latest/contents.html for details
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct ELF
{
    public byte EI_MAG0, EI_MAG1, EI_MAG2, EI_MAG3, EI_CLASS, EI_DATA, EI_VERSION;
    byte unused1, unused2, unused3, unused4, unused5, unused6, unused7, unused8, unused9;
    public ushort e_type;
    public ushort e_machine;
    public uint e_version;
    public uint e_entry;
    public uint e_phoff;
    public uint e_shoff;
    public uint e_flags;
    public ushort e_ehsize;
    public ushort e_phentsize;
    public ushort e_phnum;
    public ushort e_shentsize;
    public ushort e_shnum;
    public ushort e_shstrndx;
}

class Program
{
    static void Main(string[] args)
    {
        if (args.Length != 1) {
          Console.WriteLine("Usage: ReadElf <elf file>");
          return;
        }
        
        string elfFilename = args[0];
        using (FileStream strm = new FileStream(elfFilename, FileMode.Open))
        {
            ELF elfHeader = new ELF();
            byte[] data = new byte[Marshal.SizeOf(elfHeader)];
          
            // Read ELF header data
            strm.Read(data, 0, data.Length);
            // Convert to struct
            elfHeader = ByteArrayToStructure<ELF>(data);
          
            Console.WriteLine("Entry point: " + elfHeader.e_entry.ToString("X4"));
            Console.WriteLine("Number of program header entries: " + elfHeader.e_phnum);

            // Read first program header entry
            strm.Seek(elfHeader.e_phoff, SeekOrigin.Begin);
            data = new byte[elfHeader.e_phentsize];
            strm.Read(data, 0, (int)elfHeader.e_phentsize);
          
            // Now, do something with it ... see cppreadelf for a hint

        }
    }

    // Converts a byte array to a struct
    static T ByteArrayToStructure<T>(byte[] bytes) where T : struct
    {
        GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
        T stuff = (T)Marshal.PtrToStructure(handle.AddrOfPinnedObject(),
            typeof(T));
        handle.Free();
        return stuff;
    }

}
