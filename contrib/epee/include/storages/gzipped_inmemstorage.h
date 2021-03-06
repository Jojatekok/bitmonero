// Copyright (c) 2014, The Monero Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Parts of this file are originally copyright (c) 2006-2013, Andrey N. Sabelnikov

#ifndef _GZIPPED_INMEMSTORAGE_H_
#define _GZIPPED_INMEMSTORAGE_H_

#include "zlib_helper.h"
namespace epee
{
namespace StorageNamed
{

	template<class t_base_storage>
	class gziped_storage: public t_base_storage
	{
	public: 
		size_t	PackToSolidBuffer(std::string& targetObj)
		{
			size_t res = t_base_storage::PackToSolidBuffer(targetObj);
			if(res <= 0)
				return res;

			if(!zlib_helper::pack(targetObj))
				return 0;
		
			return targetObj.size();
		}

		size_t		LoadFromSolidBuffer(const std::string& pTargetObj)
		{
			std::string buff_to_ungzip = pTargetObj;
			if(zlib_helper::unpack(buff_to_ungzip))
				return t_base_storage::LoadFromSolidBuffer(buff_to_ungzip);

			return 0;
		}

	private:
	};

}
}

#endif 