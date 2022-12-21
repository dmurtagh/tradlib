//
//  DSPCommon.hpp
//  tradlib
//
//  Created by damien murtagh on 12/20/22.
//

#pragma once

#include <vector>
#include <memory>

using namespace std;

namespace tradlib
{
    typedef shared_ptr<vector<float>> SharedFloatVec;

    typedef shared_ptr<vector<int>> SharedIntVec;
}
