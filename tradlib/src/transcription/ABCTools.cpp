//
//  ABCTools.cpp
//  tradlib
//
//  Created by damien murtagh on 12/23/22.
//

#include "ABCTools.hpp"

using namespace tradlib;


ABCTools::ABCTools()
{
}

/**
    Modification happens in place.
 */
bool ABCTools::stripAll(std::string & notes)
{
    ABCTools::stripComments(notes);
    ABCTools::stripWhiteSpace(notes);
    ABCTools::expandLongNotes(notes);
    
    if (ABCTools::expandParts(notes) == false)
        return false;
    
    ABCTools::stripBarDivisions(notes);
    ABCTools::removeTripletMarks(notes);
    ABCTools::removeExtraNotation(notes);
    
    return true;
}

void ABCTools::test()
{
    std::string test = "";
    
    /*
     test += "Beed e2BA|GBAF GFEF|Dddc d2AG|FAGE FEDF|";
     test += "Eeed efge|fedf e2BA|GABG FGAF|BGAF GEE2:| ";
     test += "|:gfef gebe|gebe gfec|d2fd Adfd|cdAG FDEF|";
     test += "GABG FGAF|E2ef gfed|BcdB A2FA|BGAF GEE2:|";
     test += "|:Beed e2BA|GBAF GFEF|Dddc d2AG|FAGE FEDF|";
     test += "Eeed efge|fedf e2BA|GABG FGAF|BGAF GEE2:|";
     test += "|:gfef gebe|gebe gfec|d2fd Adfd|cdAG FDEF|";
     test += "GABG FGAF|E2ef gfed|BcdB A2FA|BGAF GEE2:|";
     */
    /*test += "~G3A Bcdg|eB~B2 egdB|GFGA BcdB|AcBA GEDE|";
     test += "GFGA Bcdg|eB~B2 d2ef|g2af gedB|1 AcBA GEDE:|2 AcBA GED2||";
     test += "|:gfga gedg|eB~B2 d2ef|g2af gedB|AcBA GED2|";
     test += "~g3a gedg|eB~B2 d2ef|gbaf gedB|1 AcBA GED2:|2 AcBA GEDE||";
     
     test += "~G3A Bd~d2|eB~B2 eBdB|~G3A B2dB|AcBA GEDE|";
     test += "~G3A ~B3d|eB~B2 d2ef|gbaf gedB|1 AcBA GEDE:|2 AcBA GED2||";
     test += "|:~g3a gfed|eB~B2 d2ef|gfga gedB|AcBA GED2|";
     test += "gfga gfed|eB~B2 d2ef|g2af gedB|1 AcBA GED2:|2 AcBA GEDE||";
     */
    test += "A2FA df~f2|dfef dB~B2|A2FA dffe|dBAG FDDB|";
    test += "A2FA df~f2|afef dB~B2|A2FA dffe|dBAG FDD2||";
    test += "|:a2~a2 afdf|afef dB~B2|fbba bafa|bfaf feef|";
    test += "bf~f2 af~f2|afef dB~B2|A2FA dffe|1 dBAF ADD2:|2 dBAF ADDB||";
    test += "A2FA dfef|df (3efe dB~B2|A2FA defe|dBAG FD~D2|";
    test += "A2FA df~f2|afgf efdB|(3ABA FA defe|dBAG FD~D2||";
    test += "|:~a3z afdf|afef dB~B2|fbba babc'|d'c'ba feef|";
    test += "bf~f2 af~f2|afef efdB|(3ABA FA defe|1 dBAF ADD2:|2 dBAF ADD2||";
    
    ABCTools::stripComments(test);
    ABCTools::stripWhiteSpace(test);
    ABCTools::expandLongNotes(test);
    ABCTools::expandParts(test);
    ABCTools::stripBarDivisions(test);
    ABCTools::removeTripletMarks(test);
    
    std::transform(test.begin(), test.end(),test.begin(), ::toupper);
    
    Logger::log(test);
}

void ABCTools::removeSubstrs(string& s, string& p)
{
  string::size_type n = p.length();
  for (string::size_type i = s.find(p);
      i != string::npos;
      i = s.find(p))
      s.erase(i, n);
}

void ABCTools::removeSubstrs(string& s, const char * p)
{
    std::string str(p);
    removeSubstrs(s, str);
}

void ABCTools::removeExtraNotation(std::string notes)
{
    removeSubstrs(notes, ">");
    removeSubstrs(notes, "/");
    removeSubstrs(notes, "\\");
    removeSubstrs(notes, "(");
    removeSubstrs(notes, ")");
    removeSubstrs(notes, "/");
    removeSubstrs(notes, "-");
    removeSubstrs(notes, "!");
    removeSubstrs(notes, "_");
    
    
    removeSubstrs(notes, "\\[.\\]");
    removeSubstrs(notes, "\\[..\\]");
    removeSubstrs(notes, "\\[...\\]");
    removeSubstrs(notes, "\\[....\\]");
    removeSubstrs(notes, "\\[.....\\]");
    removeSubstrs(notes, "\\[.*\\]");
    
    
    std::string temp;
    for (int i = 0 ; i < notes.size() ; i ++)
    {
        char cur = notes[i];
        if (((cur >= 'A') && (cur <= 'G')) || ((cur >= 'a') && (cur <= 'g')))
        {
            temp.push_back(cur);
        }
        
    }
    notes = temp;
}

/**
    Expansion happens in place.
    Returns success or failure
 */
bool ABCTools::expandParts(std::string notes)
{
    size_t start = 0;
    size_t end = 0;
    const std::string endToken = ":|";
    int count = 0;
    while (true)
    {
        if (count > 10)
        {
            return false;
        }
        count ++;
        end = notes.find(endToken);
        
        if (end == std::string::npos)
        {
            break;
        }
        else
        {
            size_t newStart = notes.find_last_of("|:", end);
            if (newStart != std::string::npos)
            {
                start = newStart + 1;
            }
            
            // Handling edge cases like this one (See the ABC file for X:1) https://thesession.org/tunes/21207
            bool hasRepititionVariation = (notes.size() > end + 2) && isdigit(notes[end + 2]);
            bool endColonMissing = false;
            if (notes[start] == '|' && end < start)
            {
                // Search for an end with |2 (sometimes the "|:" seems to be missing)
                end = notes.find("|2");
                if (end == std::string::npos)
                {
                    break;
                }
                endColonMissing = true;
                hasRepititionVariation = (notes.size() > end + 1) && isdigit(notes[end + 1]);
                start = start + 1;
            }
            
            if (hasRepititionVariation)
            {
                int numSpecialBars = 1;
                std::string expanded;
                size_t normalPart = notes.find_last_of("|", end);
                if (endColonMissing)
                {
                    normalPart = notes.find_last_of("|", end - 1);
                }
                
                if (! isdigit(notes[normalPart + 1]))
                {
                    normalPart = notes.find_last_of("|", normalPart - 1);
                    numSpecialBars ++;
                }
                expanded.append(notes.substr(start, normalPart - start));
                expanded.append("|");
                expanded.append(notes.substr(normalPart + 2, end - (normalPart+2)));
                size_t secondTime = end;
                while ((numSpecialBars --) > 0)
                {
                    secondTime = notes.find("|", secondTime + 2);
                }
                expanded.append("|");
                expanded.append(notes.substr(start, normalPart-start));
                expanded.append("|");
                if (endColonMissing)
                {
                    expanded.append(notes.substr(end + 2, secondTime - (end+2)));
                }
                else
                {
                    expanded.append(notes.substr(end + 3, secondTime - (end+3)));
                }
                expanded.append("|");
                notes.replace(start, secondTime, expanded);
            }
            else
            {
                std::string expanded;
                expanded.append(notes.substr(start, end - start));
                expanded.append("|");
                expanded.append(notes.substr(start, end - start));
                notes.replace(start, end + 2, expanded);
                start = start + expanded.size();
            }
        }
    }
    
    return true;
}

/**
    Modification happens in place.
 */
void ABCTools::stripBarDivisions(std::string & notes)
{
    std::string stripped;
    for (int i = 0 ;  i < notes.size(); i ++)
    {
        char c  = notes[i];
        if ((c != '|') && (c != ':'))
        {
            stripped.push_back(c);
        }
    }
    notes = stripped;
}

/**
    Modification happens in place.
 */
void ABCTools::removeTripletMarks(std::string & notes)
{
    std::string stripped;
    
    // First remove ornaments
    for (int i = 0 ;  i < notes.length(); i ++)
    {
        char c  = notes[i];
        if ((c == '(') && isdigit(notes[i+1]))
        {
            i +=1;
            continue;
        }
        stripped.push_back(c);
    }
    notes = stripped;
}

/**
    Modification happens in place.
 */
void ABCTools::expandLongNotes(std::string & notes)
{
    std::string stripped;
    // First remove ornaments
    bool inOrnament = false;
    for (int i = 0 ;  i < notes.size(); i ++)
    {
        char c  = notes[i];
        if (c == '{')
        {
            inOrnament = true;
            continue;
        }
        if (c == '}')
        {
            inOrnament = false;
            continue;
        }
        
        if ((c != '~') && ! inOrnament && (c != ',') && (c != '=') && (c != '^') && (c != '\''))
        {
            stripped.push_back(c);
        }
    }
    for (int i = 1 ;  i < stripped.size(); i ++)
    {
        char c  = stripped[i];
        char p = stripped[i -1];
        // Its a long note
        if (isdigit(c) && isalpha(p))
        {
            std::string expanded = "";
            int howMany = c - '0';
            for (int j = 0 ; j < howMany; j ++)
            {
                expanded += p;
            }
            stripped.replace(i - 1, i + 1, expanded);
        }
        // Expand Z's
        /*
         if (c == 'z')
         {
         retValue.replace(i, i, "" + p);
         }
         */
    }
    notes = stripped;
}

/**
    Modification happens in place.
 */
void ABCTools::stripNonNotes(std::string & notes)
{
    stripComments(notes);
    std::string stripped;
    for (int i = 0 ;  i < notes.size(); i ++)
    {
        char c  = notes[i];
        
        if (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) || ((c >= '1') && (c <= '9')) || (c == '('))
        {
            stripped.push_back(c);
        }
    }
    notes = stripped;
}

/**
    Modification happens in place.
 */
void ABCTools::stripWhiteSpace(std::string & notes)
{
    std::string stripped;
    int i = 0;
    while (i < notes.size())
    {
        if ((notes[i] != ' ') &&  (notes[i] != '\r') && (notes[i] != '\n'))
        {
            stripped.push_back(notes[i]);
        }
        i ++;
    }
    
    notes = stripped;
}

/**
    Modification happens in place.
 */
void ABCTools::stripComments(std::string & notes)
{
    std::string stripped;
    
    int i = 0;
    bool inComment = false;
    while (i < notes.size())
    {
        if (notes[i] == '"')
        {
            if (inComment)
            {
                inComment = false;
                i ++;
                continue;
            }
            else
            {
                inComment = true;
            }
        }
        if (!inComment)
        {
            stripped.push_back(notes[i]);
        }
        i ++;
    }
    notes = stripped;
}
