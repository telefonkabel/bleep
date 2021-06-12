//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file bleep.cpp
/// \brief Provides main function for this game.
//==============================================================================

/// \brief Header guard for olcPixelGameEngine.
#define OLC_PGE_APPLICATION
/// \brief Header guard for olcPGEX_Sound.
#define OLC_PGEX_SOUND

//crossplatform
#ifdef _WIN32
    #define USE_WINDOWS
    #include <codeanalysis\warnings.h>
    #pragma warning(push)
    #pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
        #include "thirdParty/olcPixelGameEngine.h"
        #include "thirdParty/olcPGEX_Sound.h"
        #include "thirdParty/rapidjson/document.h"

        #include <locale>
        #include <codecvt>
    #pragma warning(pop)
#else
    #include "thirdParty/olcPixelGameEngine.h"
    #include "thirdParty/olcPGEX_Sound.h"
    #include "thirdParty/rapidjson/document.h"

    #include <unistd.h>
#endif


#include "game.h"
#include "helper/exception.h"

#include <filesystem>


/// \brief Main function.
int main()
{
    try
    {
        //get the current path of the executable platform-independently
        std::wstring wPath{};
#ifdef USE_WINDOWS
        {
            std::vector<wchar_t> buffer(0);
            DWORD len{};
            do
            {
                buffer.resize(buffer.size() + 1);
                len = GetModuleFileName(NULL, &buffer.at(0), static_cast<DWORD>(buffer.size()));
            } while (buffer.size() == len);

            if (len == 0)
                throw CException{ "GetModuleFileName doesn't get any path.", INFO };
            else
                wPath = { buffer.data() };

            //hide cursor (for windows only for now)
            ShowCursor(false);
        }
#else
        {
            std::vector<char> buffer(0);
            std::size_t len{};
            do
            {
                buffer.resize(buffer.size() + 1);
                len = readlink("/proc/self/exe", &buffer.at(0), buffer.size());
            } while (buffer.size() == len);

            if (len == 0)
                throw CException{ "readlink doesn't get any path.", INFO };
            else
            {
                std::string path{ buffer.data() };
                wPath = { path.begin(), path.end() };
            }
        }
#endif
        std::filesystem::path currentPath{ wPath };
        currentPath = currentPath.parent_path();
        if (!std::filesystem::exists(currentPath))
            throw CException{ currentPath.string() + " doesn't exist.", INFO };


        //setting up rand() - that rng is good enough for this purpose
        srand(static_cast<unsigned int>(time(0)));

        //start game
        std::cout << "Starting game." << std::endl;
        CGame game{ currentPath };
        game.Start();
    }
    catch (CException& exception)
    {
        exception.print();
    }
    catch (...)
    {
        std::cerr << "Unhandled exception in " << S(__FILE__);
    }

    return 0;
}