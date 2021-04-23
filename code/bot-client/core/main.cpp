#include "System.h"
#include "modules/BotModule.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <dlfcn.h>


void loadTest(){

    std::vector<BotModule*> modules;

    boost::filesystem::path targetDir("./");

    boost::filesystem::recursive_directory_iterator iter(targetDir), eod;

    BOOST_FOREACH(boost::filesystem::path const& i, std::make_pair(iter, eod)){
        std::cout << " === : " <<i.string() << " - " << i.extension() <<  std::endl;
        if (!is_regular_file(i)  ||   i.extension() != ".so"){
            continue;
        }

#ifdef WIN32
#else
        void *handle = dlopen (i.string().c_str(), RTLD_LAZY);
        auto pluggableModule = (getInstance_t)dlsym(handle, "_Z11getInstancev");
        //getInstance_t pluggableModule = (getInstance_t)dlsym(handle, "getInstance");
#endif
        BotModule *mod = pluggableModule();
        std::cout << "i'm  " << mod->name() << std::endl;
        modules.push_back(mod);
    }

}


int main() {

    System sys;
    sys.run();
    return 0;
}
