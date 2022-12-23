//
//  FileLogger.h
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

namespace tradlib
{
    class FileLogger
    {
    public:
        
        virtual void deleteLog(const std::string & fileName) = 0;
        
        virtual void log(const std::string & fileName, const std::string & txt) = 0;
    };
}
