  
  
 

/**************************************************************************************** 
*
* This code was generated by the Command Creator tool for the ChainsAPM project. 
* If manual changes are made to the code they could be lost.
*
****************************************************************************************/
#pragma once
#include "stdafx.h"
#include "commands\SendInjectionMetadata.h"
 

namespace Commands
{
	SendInjectionMetadata::SendInjectionMetadata(__int64 timestamp, std::vector<char> injectionmetadata, std::vector<char> injectionil)
		: timestamp(timestamp), code(0x0009), hasEncoded(false), InjectionMetadata(injectionmetadata), InjectionIL(injectionil) 
	{
	}

	SendInjectionMetadata::~SendInjectionMetadata()
	{
	}

	std::wstring SendInjectionMetadata::Name()
	{
		return L"Send Injection Metadata";
	}

	std::wstring SendInjectionMetadata::Description()
	{
		return L"Sends the Metadata blob across the wire";
	}

	std::shared_ptr<std::vector<char>> SendInjectionMetadata::Encode()
	{
		if (!hasEncoded)
		{
			// The following items will be resued 
			auto hash = std::hash<std::wstring>();
			size_t hashout = 0;		// Temp var for string hashes
			size_t strlen = 0;		// Temp var for string length
			size_t strbytes = 0;	// Temp var for string bytes

			size_t size = sizeof(__int32)	// len
				+ sizeof(short)				// code
				+ sizeof(__int64);			// timestamp

			// Begin the dynamically generated sizes
			size += sizeof(char) * InjectionMetadata.size(); // Size of InjectionMetadata vector
			
			size += sizeof(char) * InjectionIL.size(); // Size of InjectionIL vector
			
			// End the dynamically generated sizes  
			size += sizeof(short);	//Terminator

			auto vector = new char[size]; 
			short term = 0;
			
			// 
			auto v2 = (char*)memcpy(vector, &size, sizeof(__int32));  
			v2 += sizeof(__int32); 
			memcpy(v2, &code, sizeof(short));
			v2 += sizeof(short);
			memcpy(v2, &timestamp, sizeof(__int64));
			v2 += sizeof(__int64);

			// Begin the dynamically generated byte copies    
			
			for (auto item : InjectionMetadata) {
			     
			    memcpy(v2, &item, sizeof(char));  
			    v2 += sizeof(char); }

			
			for (auto item : InjectionIL) {
			     
			    memcpy(v2, &item, sizeof(char));  
			    v2 += sizeof(char); }

			// End the dynamically generated byte copies

			// Terminator
			memcpy(v2, &term, sizeof(short));
			v2 += sizeof(short);

			hasEncoded = true;

			internalvector = std::make_shared<std::vector<char>>(vector, v2);
		}
		return internalvector;
	}

	std::shared_ptr<ICommand> SendInjectionMetadata::Decode(std::shared_ptr<std::vector<char>> &data)      
	{
		auto hash = std::hash<std::wstring>();

		auto ptr = data->data();

		auto len = *(unsigned int*)ptr;  
		ptr += sizeof(unsigned int);

		auto code = *(short*)ptr;
		ptr += sizeof(short);

		auto local_timestamp = *(__int64*)ptr;
		ptr += sizeof(__int64);

		
		size_t InjectionMetadata_count = *(__int32*)ptr;
		ptr += sizeof(__int32);
		
		auto local_InjectionMetadata = std::vector<char>();
		
		for (size_t iInjectionMetadata = 0; iInjectionMetadata < InjectionMetadata_count; iInjectionMetadata++) {
		    auto value = *(char*)ptr;
		    ptr += sizeof(char); 
		    local_InjectionMetadata.emplace_back(value);
		}

		
		size_t InjectionIL_count = *(__int32*)ptr;
		ptr += sizeof(__int32);
		
		auto local_InjectionIL = std::vector<char>();
		
		for (size_t iInjectionIL = 0; iInjectionIL < InjectionIL_count; iInjectionIL++) {
		    auto value = *(char*)ptr;
		    ptr += sizeof(char); 
		    local_InjectionIL.emplace_back(value);
		}


		// Terminator
		auto term = *(short*)ptr;
		ptr += sizeof(short);
		return std::make_shared<SendInjectionMetadata>(local_timestamp,  local_InjectionMetadata ,  local_InjectionIL );
	}
}
