#include <boost/foreach.hpp>

#include "frl_opc.h"
#include "opc/da/frl_opc_da_server_browser.h"
#include "console_std/frl_iostream.h"
#include "stream_std/frl_ostream.h"
#include <tchar.h>

#pragma comment(lib,"Rpcrt4.lib")

class Callback : public IOPCDataCallback
{
public:

	Callback() 
	{
		m_ulRefs = 1;
	}

	//==========================================================================
	// IUnknown

	// QueryInterface
	STDMETHODIMP QueryInterface(REFIID iid, LPVOID* ppInterface) 
	{
		if (ppInterface == NULL)
		{
			return E_INVALIDARG;
		}

		if (iid == IID_IUnknown)
		{
			*ppInterface = dynamic_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}

		if (iid == IID_IOPCDataCallback)
		{
			*ppInterface = dynamic_cast<IOPCDataCallback*>(this);
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	// AddRef
	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement((LONG*)&m_ulRefs); 
	}

	// Release
	STDMETHODIMP_(ULONG) Release()
	{
		ULONG ulRefs = InterlockedDecrement((LONG*)&m_ulRefs); 

		if (ulRefs == 0) 
		{ 
			delete this; 
			return 0; 
		} 

		return ulRefs; 
	}

	//==========================================================================
	// IOPCDataCallback

	// OnDataChange
	STDMETHODIMP OnDataChange(
		DWORD       dwTransid, 
		OPCHANDLE   hGroup, 
		HRESULT     hrMasterquality,
		HRESULT     hrMastererror,
		DWORD       dwCount, 
		OPCHANDLE * phClientItems, 
		VARIANT   * pvValues, 
		WORD      * pwQualities,
		FILETIME  * pftTimeStamps,
		HRESULT   * pErrors
		)
	{

		for (DWORD ii = 0; ii < dwCount; ii++)
		{
			VARIANT vValue;
			VariantInit(&vValue);

			if (SUCCEEDED(VariantChangeType(&vValue, &(pvValues[ii]), NULL, VT_BSTR)))
			{
				using namespace frl;
				console_std::Out << FRL_STR("Handle =") << phClientItems[ii] << FRL_STR(", value=") << vValue.bstrVal;
				VariantClear(&vValue);
			}
		}

		return S_OK;
	}

	// OnReadComplete
	STDMETHODIMP OnReadComplete(
		DWORD       dwTransid, 
		OPCHANDLE   hGroup, 
		HRESULT     hrMasterquality,
		HRESULT     hrMastererror,
		DWORD       dwCount, 
		OPCHANDLE * phClientItems, 
		VARIANT   * pvValues, 
		WORD      * pwQualities,
		FILETIME  * pftTimeStamps,
		HRESULT   * pErrors
		)
	{
		return S_OK;
	}

	// OnWriteComplete
	STDMETHODIMP OnWriteComplete(
		DWORD       dwTransid, 
		OPCHANDLE   hGroup, 
		HRESULT     hrMastererr, 
		DWORD       dwCount, 
		OPCHANDLE * pClienthandles, 
		HRESULT   * pErrors
		)
	{
		return S_OK;
	}


	// OnCancelComplete
	STDMETHODIMP OnCancelComplete(
		DWORD       dwTransid, 
		OPCHANDLE   hGroup
		)
	{
		return S_OK;
	}

private:

	ULONG m_ulRefs;
};

int main( int argc, char*argv[] )
{
	using namespace frl;
	CoInitialize( NULL );

	std::vector<frl::String> list_quick_all;
	opc::da::ServerBrowser::getAllServerListForce( list_quick_all );
	console_std::Out << FRL_STR("Accessible OPC DA servers (quick):\n");
	BOOST_FOREACH( String server, list_quick_all )
	{
		console_std::Out << server << FRL_STR("\n");
	}
	console_std::Out << std::endl;

	std::vector<frl::String> list_da1;
	opc::da::ServerBrowser::getServerListDA1( list_da1 );
	console_std::Out << FRL_STR("Accessible OPC DA1 servers:\n");
	BOOST_FOREACH( String server, list_da1 )
	{
		console_std::Out << server << FRL_STR("\n");
	}
	console_std::Out << std::endl;

	std::vector<frl::String> list_da2;
	opc::da::ServerBrowser::getServerListDA2( list_da2 );
	console_std::Out << FRL_STR("Accessible OPC DA2 servers:\n");
	BOOST_FOREACH( String server, list_da2 )
	{
		console_std::Out << server << FRL_STR("\n");
	}
	console_std::Out << std::endl;

	std::vector<frl::String> list_da3;
	opc::da::ServerBrowser::getServerListDA3( list_da3 );
	console_std::Out << FRL_STR("Accessible OPC DA3 servers:\n");
	BOOST_FOREACH( String server, list_da3 )
	{
		console_std::Out << server << FRL_STR("\n");
	}

	std::vector<frl::String> list_all_force;
	opc::da::ServerBrowser::getAllServerListForce( list_all_force );
	console_std::Out << FRL_STR("Accessible OPC DA servers:\n");
	BOOST_FOREACH( String server, list_all_force )
	{
		console_std::Out << server << FRL_STR("\n");
	}
	console_std::Out << std::endl;

	{
	opc::da::Client client;
	client.Connect( FRL_STR("Serg Baburin.SERVER_TEST.0.1") );
	console_std::Out << client.getServerStatus() << std::endl;
	}

	{
	opc::da::Client client;
	client.Connect( FRL_STR("Serg Baburin.SERVER_TEST.0.1"), FRL_STR("localhost") );
	console_std::Out << client.getServerStatus() << std::endl;
	}

	CoUninitialize();
	return 0;
}
