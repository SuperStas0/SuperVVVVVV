#include "Supertext.h"

#include <map>
#include <string>
#include <tinyxml2.h>

#include "Localization.h"
#include "LocalizationStorage.h"
#include "XMLUtils.h"

namespace supertext
{
    static std::map<std::string, std::string> texts;
    static std::string loaded_lang;
    static bool loaded = false;

    static void load(void)
    {
        texts.clear();
        loaded_lang = loc::lang;
        loaded = true;

        if (loc::lang == "en")
        {
            return;
        }

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLHandle hDoc(&doc);
        tinyxml2::XMLElement* pElem;

        if (!loc::load_lang_doc("super", doc))
        {
            return;
        }

        FOR_EACH_XML_ELEMENT(hDoc, pElem)
        {
            EXPECT_ELEM(pElem, "string");

            const char* eng = pElem->Attribute("english");
            const char* tra = pElem->Attribute("translation");

            if (eng == NULL || tra == NULL || tra[0] == '\0')
            {
                continue;
            }

            texts[eng] = tra;
        }
    }

    const char* get(const char* eng)
    {
        if (!loaded || loaded_lang != loc::lang)
        {
            load();
        }

        const std::map<std::string, std::string>::const_iterator it = texts.find(eng);
        if (it == texts.end())
        {
            return eng;
        }
        return it->second.c_str();
    }
}
