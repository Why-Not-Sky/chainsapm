#pragma once
#include "stdafx.h"
#include "profilermain.h"
#include "MetadataHelpers.h"
#define NAME_BUFFER_SIZE 2048

Cprofilermain * g_ProfilerCallback = NULL;
MetadataHelpers * g_MetadataHelpers = NULL;

std::map<FunctionID, FunctionInfo> * g_FunctionSet = NULL;
std::map<ThreadID, std::queue<ThreadStackItem>> * g_ThreadStackMap = NULL;
std::map<ThreadID, int> * g_ThreadStackDepth = NULL;
std::unordered_set<std::wstring> * g_FunctionNameSet = NULL;
CRITICAL_SECTION g_ThreadingCriticalSection;


EXTERN_C void FunctionEnter2_Wrapper_x64(FunctionID funcId, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_RANGE *argumentInfo);

EXTERN_C void FunctionLeave2_Wrapper_x64(FunctionID funcId, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_RANGE *argumentInfo);

EXTERN_C void FunctionTail2_Wrapper_x64(FunctionID funcId, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_RANGE *argumentInfo);

// Static methods for specific profiler callbacks. Namely the Mapper and Enter/Leave/Tail
namespace StaticProfilerMethods
{

	// Mapper function to create the collection of FunctionInfo items
	UINT_PTR __stdcall Mapper1(FunctionID funcId, BOOL *pbHookFunction)
	{
		EnterCriticalSection(&g_ThreadingCriticalSection);
		FunctionInfo funcInfo;
		g_MetadataHelpers->GetFunctionInformation(funcId, &funcInfo);
		g_FunctionSet->insert(std::pair<FunctionID, FunctionInfo>(funcId, funcInfo));
		*pbHookFunction = TRUE;
		LeaveCriticalSection(&g_ThreadingCriticalSection);
		//EnterCriticalSection(&g_ThreadingCriticalSection);
		//std::map<FunctionID, FunctionInfo>::const_iterator it = g_FunctionSet->find(funcId);
		//if (it == g_FunctionSet->end())
		//{

		//	// declared in this block so they are not created if the function is found

		//	FunctionInfo funcInfo;

		//	g_MetadataHelpers->GetFunctionInformation(funcId, &funcInfo);
		//	std::unordered_set<std::wstring>::const_iterator findName =
		//		g_FunctionNameSet->find(funcInfo.FunctionName());
		//	if (findName != g_FunctionNameSet->end())
		//	{
		//		g_FunctionSet->insert(std::pair<FunctionID, FunctionInfo>(funcId, funcInfo));
		//		*pbHookFunction = TRUE;
		//	}
		//	else {
		//		*pbHookFunction = FALSE;
		//	}

		//}
		//LeaveCriticalSection(&g_ThreadingCriticalSection);
		return (UINT_PTR)funcId;

	}

	// Implementation of Mapper2. Just calls Mapper1
	UINT_PTR __stdcall Mapper2(FunctionID funcId, void * clientData, BOOL *pbHookFunction)
	{
		return Mapper1(funcId, pbHookFunction);
	}

	// Enter hook function for creating shadow stacks
	EXTERN_C void FunctionEnter2_CPP_Helper(FunctionID funcId, UINT_PTR clientData,
		COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo)
	{

		/*
		MSDN Article that describes the ELT methods and what COR flags need to be set.
		http://msdn.microsoft.com/en-us/magazine/cc300553.aspx
		*/

		TimerItem ti(ThreadStackReason::ENTER);
		EnterCriticalSection(&g_ThreadingCriticalSection);
		std::map<FunctionID, FunctionInfo>::const_iterator it = g_FunctionSet->find(funcId);
		if (it != g_FunctionSet->end())
		{
			const FunctionInfo *fi = &it->second;
			ThreadID threadId;

			g_MetadataHelpers->GetCurrentThread(&threadId);
			std::map<ThreadID, std::queue<ThreadStackItem>>::iterator itStack = g_ThreadStackMap->find(threadId);

			if (itStack != g_ThreadStackMap->end())
			{
				if (itStack->second.size() > 0)
				{
					g_ThreadStackDepth->at(threadId)++;
				}
				ThreadStackItem tsi(threadId, funcId, ThreadStackReason::ENTER, (byte*)argumentInfo);
				tsi.Depth(g_ThreadStackDepth->at(threadId));
				ti.AddThreadStackItem(&tsi);
				itStack->second.push(*new ThreadStackItem(tsi));
			}

			// TODO extract argument 
		}
		LeaveCriticalSection(&g_ThreadingCriticalSection);
		//TODO Implement function callbacks
	}

	// Leave hook function for creating shadow stacks
	EXTERN_C void FunctionLeave2_CPP_Helper(FunctionID funcId, UINT_PTR clientData,
		COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_RANGE *argumentRange)
	{
		TimerItem ti(ThreadStackReason::EXIT);
		EnterCriticalSection(&g_ThreadingCriticalSection);
		std::map<FunctionID, FunctionInfo>::const_iterator it = g_FunctionSet->find(funcId);
		if (it != g_FunctionSet->end())
		{

			ThreadID threadId;

			g_MetadataHelpers->GetCurrentThread(&threadId);
			std::map<ThreadID, std::queue<ThreadStackItem>>::iterator itStack = g_ThreadStackMap->find(threadId);
			if (itStack != g_ThreadStackMap->end())
			{
				if (itStack->second.size() > 0)
				{
					if (g_ThreadStackDepth->at(threadId) > 0)
					{
						g_ThreadStackDepth->at(threadId)--;
					}

				}
				ThreadStackItem* tsi = &itStack->second.back();
				tsi->ItemStackReturnValue((byte*)argumentRange);
				ti.AddThreadStackItem(tsi);
			}

			// TODO extract argument 
		}
		LeaveCriticalSection(&g_ThreadingCriticalSection);
		//TODO Implement function callbacks
	}
	
	// Tail hook function for creating shadow stacks
	EXTERN_C void FunctionTail2_CPP_Helper(FunctionID funcId, UINT_PTR clientData,
		COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo)
	{
		TimerItem ti(ThreadStackReason::TAIL);
		EnterCriticalSection(&g_ThreadingCriticalSection);
		std::map<FunctionID, FunctionInfo>::const_iterator it = g_FunctionSet->find(funcId);
		if (it != g_FunctionSet->end())
		{
			ThreadID threadId;

			g_MetadataHelpers->GetCurrentThread(&threadId);
			std::map<ThreadID, std::queue<ThreadStackItem>>::iterator itStack = g_ThreadStackMap->find(threadId);
			int depth = 0;
			if (itStack != g_ThreadStackMap->end())
			{
				if (itStack->second.size() > 0)
				{
					g_ThreadStackDepth->at(threadId)--;
				}
				ThreadStackItem tsi(threadId, funcId, ThreadStackReason::TAIL, (byte*)argumentInfo);
				tsi.Depth(g_ThreadStackDepth->at(threadId));
				ti.AddThreadStackItem(&tsi);
				itStack->second.push(*new ThreadStackItem(tsi));
			}
		}
		LeaveCriticalSection(&g_ThreadingCriticalSection);
		// TODO extract argument 
	}
	//TODO Implement function callbacks

}

