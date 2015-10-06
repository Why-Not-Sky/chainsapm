  
  
 

/**************************************************************************************** 
*
* This code was generated by the Command Creator tool for the ChainsAPM project. 
* If manual changes are made to the code they could be lost.
*
****************************************************************************************/
#pragma once
#include "stdafx.h"
#include "commands\DefineInstrumentationMethods.h"
 

namespace Commands
{
	DefineInstrumentationMethods::DefineInstrumentationMethods(__int64 timestamp, std::wstring assemblyname, std::wstring typenamestring, std::wstring methodname, std::wstring injectionmethodname, std::vector<char> injectionmethodilbytes)
		: timestamp(timestamp), code(0x0008), hasEncoded(false), AssemblyName(assemblyname), TypeNameString(typenamestring), MethodName(methodname), InjectionMethodName(injectionmethodname), InjectionMethodILBytes(injectionmethodilbytes) 
	{
	}

	DefineInstrumentationMethods::~DefineInstrumentationMethods()
	{
	}

	std::wstring DefineInstrumentationMethods::Name()
	{
		return L"Define Instrumentation Method";
	}

	std::wstring DefineInstrumentationMethods::Description()
	{
		return L"This command will define a single instrumentation method.";
	}

	std::shared_ptr<std::vector<char>> DefineInstrumentationMethods::Encode()
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
			size_t strlen_AssemblyNamelen = AssemblyName.length();
			size_t strbytes_AssemblyNamelen = (strlen_AssemblyNamelen * sizeof(wchar_t)) + sizeof(wchar_t);
			size += sizeof(__int32); // String Length 
			size += sizeof(__int64); // String Hash
			size += strbytes_AssemblyNamelen; // String Bytes

			size_t strlen_TypeNameStringlen = TypeNameString.length();
			size_t strbytes_TypeNameStringlen = (strlen_TypeNameStringlen * sizeof(wchar_t)) + sizeof(wchar_t);
			size += sizeof(__int32); // String Length 
			size += sizeof(__int64); // String Hash
			size += strbytes_TypeNameStringlen; // String Bytes

			size_t strlen_MethodNamelen = MethodName.length();
			size_t strbytes_MethodNamelen = (strlen_MethodNamelen * sizeof(wchar_t)) + sizeof(wchar_t);
			size += sizeof(__int32); // String Length 
			size += sizeof(__int64); // String Hash
			size += strbytes_MethodNamelen; // String Bytes

			size_t strlen_InjectionMethodNamelen = InjectionMethodName.length();
			size_t strbytes_InjectionMethodNamelen = (strlen_InjectionMethodNamelen * sizeof(wchar_t)) + sizeof(wchar_t);
			size += sizeof(__int32); // String Length 
			size += sizeof(__int64); // String Hash
			size += strbytes_InjectionMethodNamelen; // String Bytes

			size += sizeof(char) * InjectionMethodILBytes.size(); // Size of InjectionMethodILBytes vector
			
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
			size_t strlen_AssemblyNamedata = AssemblyName.length();
			size_t strbytes_AssemblyNamedata = (strlen_AssemblyNamedata * sizeof(wchar_t)) + sizeof(wchar_t);
			hashout = hash(AssemblyName);
			 
			
			memcpy(v2, &strlen_AssemblyNamedata, sizeof(__int32));
			v2 += sizeof(__int32);
			memcpy(v2, &hashout, sizeof(size_t));
			v2 += sizeof(__int64);
			memcpy(v2, AssemblyName.data(), strbytes_AssemblyNamedata);
			v2 += strbytes_AssemblyNamedata;
			size_t strlen_TypeNameStringdata = TypeNameString.length();
			size_t strbytes_TypeNameStringdata = (strlen_TypeNameStringdata * sizeof(wchar_t)) + sizeof(wchar_t);
			hashout = hash(TypeNameString);
			 
			
			memcpy(v2, &strlen_TypeNameStringdata, sizeof(__int32));
			v2 += sizeof(__int32);
			memcpy(v2, &hashout, sizeof(size_t));
			v2 += sizeof(__int64);
			memcpy(v2, TypeNameString.data(), strbytes_TypeNameStringdata);
			v2 += strbytes_TypeNameStringdata;
			size_t strlen_MethodNamedata = MethodName.length();
			size_t strbytes_MethodNamedata = (strlen_MethodNamedata * sizeof(wchar_t)) + sizeof(wchar_t);
			hashout = hash(MethodName);
			 
			
			memcpy(v2, &strlen_MethodNamedata, sizeof(__int32));
			v2 += sizeof(__int32);
			memcpy(v2, &hashout, sizeof(size_t));
			v2 += sizeof(__int64);
			memcpy(v2, MethodName.data(), strbytes_MethodNamedata);
			v2 += strbytes_MethodNamedata;
			size_t strlen_InjectionMethodNamedata = InjectionMethodName.length();
			size_t strbytes_InjectionMethodNamedata = (strlen_InjectionMethodNamedata * sizeof(wchar_t)) + sizeof(wchar_t);
			hashout = hash(InjectionMethodName);
			 
			
			memcpy(v2, &strlen_InjectionMethodNamedata, sizeof(__int32));
			v2 += sizeof(__int32);
			memcpy(v2, &hashout, sizeof(size_t));
			v2 += sizeof(__int64);
			memcpy(v2, InjectionMethodName.data(), strbytes_InjectionMethodNamedata);
			v2 += strbytes_InjectionMethodNamedata;
			
			for (auto item : InjectionMethodILBytes) {
			     
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

	std::shared_ptr<ICommand> DefineInstrumentationMethods::Decode(std::shared_ptr<std::vector<char>> &data)      
	{
		auto hash = std::hash<std::wstring>();

		auto ptr = data->data();

		auto len = *(unsigned int*)ptr;  
		ptr += sizeof(unsigned int);

		auto code = *(short*)ptr;
		ptr += sizeof(short);

		auto local_timestamp = *(__int64*)ptr;
		ptr += sizeof(__int64);

		
		auto strlenAssemblyName = *(__int32*)ptr;
		ptr += sizeof(__int32);
		
		auto hashAssemblyName = *(__int64*)ptr;
		ptr += sizeof(__int64);
		
		auto local_AssemblyName = (wchar_t*)ptr;
		ptr += strlenAssemblyName * 2;
		
		auto strlenTypeNameString = *(__int32*)ptr;
		ptr += sizeof(__int32);
		
		auto hashTypeNameString = *(__int64*)ptr;
		ptr += sizeof(__int64);
		
		auto local_TypeNameString = (wchar_t*)ptr;
		ptr += strlenTypeNameString * 2;
		
		auto strlenMethodName = *(__int32*)ptr;
		ptr += sizeof(__int32);
		
		auto hashMethodName = *(__int64*)ptr;
		ptr += sizeof(__int64);
		
		auto local_MethodName = (wchar_t*)ptr;
		ptr += strlenMethodName * 2;
		
		auto strlenInjectionMethodName = *(__int32*)ptr;
		ptr += sizeof(__int32);
		
		auto hashInjectionMethodName = *(__int64*)ptr;
		ptr += sizeof(__int64);
		
		auto local_InjectionMethodName = (wchar_t*)ptr;
		ptr += strlenInjectionMethodName * 2;
		
		size_t InjectionMethodILBytes_count = *(__int32*)ptr;
		ptr += sizeof(__int32);
		
		auto local_InjectionMethodILBytes = std::vector<char>();
		
		for (size_t iInjectionMethodILBytes = 0; iInjectionMethodILBytes < InjectionMethodILBytes_count; iInjectionMethodILBytes++) {
		    auto value = *(char*)ptr;
		    ptr += sizeof(char); 
		    local_InjectionMethodILBytes.emplace_back(value);
		}


		// Terminator
		auto term = *(short*)ptr;
		ptr += sizeof(short);
		return std::make_shared<DefineInstrumentationMethods>(local_timestamp,  local_AssemblyName ,  local_TypeNameString ,  local_MethodName ,  local_InjectionMethodName ,  local_InjectionMethodILBytes );
	}
}

