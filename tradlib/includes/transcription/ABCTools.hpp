//
//  ABCTools.hpp
//  tradlib
//
//  Created by damien murtagh on 12/23/22.
//

#pragma once
#include <string>
#include "Logger.hpp"

using namespace std;

namespace tradlib
{
    class ABCTools
    {
    public:
        
        ABCTools();
        
        /**
            Modification happens in place.
         */
        static bool stripAll(std::string & notes);
        
        static void test();
        
    private:
        
        static void removeSubstrs(string& s, string& p);
        
        static void removeSubstrs(string& s, const char * p);
        
        static void removeExtraNotation(std::string notes);
        
        /**
            Expansion happens in place.
            Returns success or failure
         */
        static bool expandParts(std::string notes);
        
        /**
            Modification happens in place.
         */
        static void stripBarDivisions(std::string & notes);
        
        /**
            Modification happens in place.
         */
        static void removeTripletMarks(std::string & notes);
        
        /**
            Modification happens in place.
         */
        static void expandLongNotes(std::string & notes);
        
        /**
            Modification happens in place.
         */
        static void stripNonNotes(std::string & notes);
        
        /**
            Modification happens in place.
         */
        static void stripWhiteSpace(std::string & notes);
        
        /**
            Modification happens in place.
         */
        static void stripComments(std::string & notes);
    };
}
