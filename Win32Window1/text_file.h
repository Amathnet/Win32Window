#include <Windows.h>
#include <shobjidl.h> 
#include <fstream>
#include <vector>
#include <iterator>

class text_file
{
public:
    template<typename T>
    void write(std::string name, std::vector<T> vec)
    {
        std::ofstream write_file_name;
        write_file_name.open(name, std::ios::ate);
        if (write_file_name.is_open()) 
            for (T i : vec)
            {
                write_file_name << i << " ";
            }
        write_file_name.close();
    }

    template<typename T>
    std::vector<T> read(std::wstring name)
    {
        T buffer;
        std::vector<T> data;

        std::wfstream read_file_name;
        read_file_name.open(name, std::wfstream::in);
        while (read_file_name >> buffer) data.push_back(buffer);
        read_file_name.close();

        return data;
    }
};

class binary_file
{
public:
    //template<typename T>
    /* void write(std::string name, std::vector<T> vec)
    {
        std::ofstream write_file_name;

        write_file_name.open(name, std::ios::ate | ios::binary | ios::out);

        if (write_file_name.is_open()) for (T i : vec) write_file_name << i << std::endl;
        write_file_name.close();



    }
    */

};

class winapi_file : public text_file
{
public:
    std::wstring open_file_dialog()
    {
        std::wstring buffer;
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        if (SUCCEEDED(hr))
        {
            IFileOpenDialog* pFileOpen;

            // Create the FileOpenDialog object.
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

            if (SUCCEEDED(hr))
            {
                // Show the Open dialog box.
                hr = pFileOpen->Show(NULL);

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pFileOpen->GetResult(&pItem);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR pszFilePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                        // Display the file name to the user.
                        if (SUCCEEDED(hr))
                        {
                            buffer = pszFilePath;
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }
                pFileOpen->Release();
            }
            CoUninitialize();
        }
        return buffer;
    }

    std::wstring save_file_dialog()
    {
        std::wstring buffer;
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        if (SUCCEEDED(hr))
        {
            IFileOpenDialog* pFileOpen;

            // Create the FileOpenDialog object.
            hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileOpen));

            if (SUCCEEDED(hr))
            {
                // Show the Open dialog box.
                hr = pFileOpen->Show(NULL);

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pFileOpen->GetResult(&pItem);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR pszFilePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                        // Display the file name to the user.
                        if (SUCCEEDED(hr))
                        {
                            buffer = pszFilePath;
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }
                pFileOpen->Release();
            }
            CoUninitialize();
        }
        return buffer;
    }
};