//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// bleep.cpp
/// Provides the main function for this game.
//==============================================================================

//defines for third party
#define OLC_PGE_APPLICATION
#define OLC_PGEX_SOUND

//crossplatform
#ifdef _WIN32
    #define USE_WINDOWS
    #include <codeanalysis\warnings.h>
    #pragma warning(push)
    #pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
        #include <locale>
        #include <codecvt>

        #include "rapidjson/document.h"
        #include "olcPixelGameEngine.h"
        #include "olcPGEX_Sound.h"
    #pragma warning(pop)
#else
    #include <unistd.h>

    #include "rapidjson/document.h"
    #include "olcPixelGameEngine.h"
    #include "olcPGEX_Sound.h"
#endif


#include "game.h"
#include "parser.h"
#include "exception.h"

#include <filesystem>


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
                wPath = { buffer.data(), buffer.size() };
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
                std::string path{ buffer.data(), buffer.size() };
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

        //load settings
        rapidjson::Document settings{};
        CParser parser{ currentPath };
        parser.parse(settings);

        //start game
        CGame game{ currentPath, "Bleep", olc::DARK_MAGENTA, 1000000, 640, 480, 2, 2, true };
        game.Start();
    }
    catch (CException& exception)
    {
        std::cerr << exception.msg();
    }
    catch (...)
    {
        std::cerr << "Unhandled exception in bleep.cpp.";
    }

    return 0;
}