
#include <d3dcompiler.h>

#include <string>

#include <cstdio>
#include <cstdint>

int main(void)
{
    ID3DBlob *shader_blob, *error_blob;

    HMODULE lib = LoadLibrary("d3dcompiler_47.dll");
    pD3DCompile compile = (pD3DCompile)GetProcAddress(lib, "D3DCompile");

    std::string shader_source, shader_file, shader_main, shader_type;
    uint32_t flags1 = 0, flags2 = 0;
    HRESULT ret = 0;

    bool must_reset = true;

    for (;;)
    {
        if (must_reset)
        {
            shader_source = shader_file = shader_main = shader_type = "";
            shader_blob = error_blob = nullptr;
            must_reset = false;
            flags1 = flags2 = 0;
            ret = 0;
        }

        std::string tmp;
        int ch = getchar();

        if (ch < 0)
            break;

        switch (ch)
        {
        case 's':
            while ((ch = getchar()))
                shader_source += ch;
            fprintf(stderr, "[SERVER] Got %d bytes of source\n", (int)shader_source.size());
            break;

        case 'f':
            while ((ch = getchar()))
                shader_file += ch;
            fprintf(stderr, "[SERVER] Filename: %s\n", shader_file.c_str());
            break;

        case '1':
            while ((ch = getchar()))
                tmp += ch;
            flags1 = atoi(tmp.c_str());
            fprintf(stderr, "[SERVER] Flags1: %04x\n", flags1);
            break;

        case '2':
            while ((ch = getchar()))
                tmp += ch;
            flags2 = atoi(tmp.c_str());
            fprintf(stderr, "[SERVER] Flags2: %04x\n", flags2);
            break;

        case 'm':
            while ((ch = getchar()))
                shader_main += ch;
            fprintf(stderr, "[SERVER] Entry point: %s\n", shader_main.c_str());
            break;

        case 't':
            while ((ch = getchar()))
                shader_type += ch;
            fprintf(stderr, "[SERVER] Type: %s\n", shader_type.c_str());
            break;

        case 'X':
            ret = compile(shader_source.c_str(), shader_source.size(),
                          shader_file.c_str(),
                          nullptr, /* unimplemented */
                          nullptr, /* unimplemented */
                          shader_main.c_str(),
                          shader_type.c_str(),
                          flags1, flags2, &shader_blob, &error_blob);
            fprintf(stderr, "[SERVER] Result: 0x%08x\n", (int)ret);
            fprintf(stdout, "r%d%c", (int)ret, '\0');
            if (shader_blob)
            {
                fprintf(stderr, "[SERVER] Sending code\n");
                fprintf(stdout, "l%d%c", (int)shader_blob->GetBufferSize(), '\0');
                fwrite(shader_blob->GetBufferPointer(), (int)shader_blob->GetBufferSize(), 1, stdout);
                shader_blob->Release();
            }
            if (error_blob)
            {
                fprintf(stderr, "[SERVER] Sending error\n");
                fprintf(stdout, "e%d%c", (int)error_blob->GetBufferSize(), '\0');
                fwrite(error_blob->GetBufferPointer(), (int)error_blob->GetBufferSize(), 1, stdout);
                error_blob->Release();
            }
            fprintf(stdout, "q");
            fflush(stdout);
            must_reset = true;
            break;

        case 'q':
            return 0;
        }
    }
}

