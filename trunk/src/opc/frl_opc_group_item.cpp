#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include "opc/frl_opc_group_item.h"
#include "../dependency/vendors/opc_foundation/opcerror.h"
#include "opc/address_space/frl_opc_address_space.h"

using namespace frl::opc::address_space;

namespace frl
{
namespace opc
{
GroupItem::GroupItem()
	:	clientHandle( 0 ),
		actived( False ),
		requestDataType( VT_EMPTY ),
		tagRef( NULL )
{
	resetTimeStamp();
}

GroupItem::~GroupItem()
{

}

void GroupItem::Init( OPCITEMDEF &itemDef )
{
	clientHandle = itemDef.hClient;
	#if( FRL_CHARACTER == FRL_CHARACTER_UNICODE )
		itemID = itemDef.szItemID;
		accessPath = itemDef.szAccessPath;
	#else
		itemID = wstring2string( itemDef.szItemID );
		accessPath = wstring2string( itemDef.szAccessPath );
	#endif
	
	actived = ( itemDef.bActive == TRUE || itemDef.bActive == VARIANT_TRUE );
	requestDataType = itemDef.vtRequestedDataType;
}

void GroupItem::isActived( Bool activedFlag )
{
	actived = activedFlag;
}

frl::Bool GroupItem::isActived() const
{
	return actived;
}

void GroupItem::setClientHandle( OPCHANDLE handle )
{
	clientHandle = handle;
}

void GroupItem::setRequestDataType( VARTYPE type )
{
	requestDataType = type;
}

OPCHANDLE GroupItem::getClientHandle() const
{
	return clientHandle;
}

const String& GroupItem::getItemID() const
{
	return itemID;
}

const String& GroupItem::getAccessPath() const
{
	return accessPath;
}

VARTYPE GroupItem::getReguestDataType() const
{
	return requestDataType;
}

const os::win32::com::Variant& GroupItem::readValue()
{
	if( tagRef == NULL )
		tagRef = opcAddressSpace::getInstance().getLeaf( itemID );

	cachedValue = tagRef->read();
	lastChange = tagRef->getTimeStamp();
	return cachedValue;
}

HRESULT GroupItem::writeValue( const VARIANT &newValue )
{
	try
	{
		if( tagRef == NULL )
			tagRef = opcAddressSpace::getInstance().getLeaf( itemID );
	}
	catch( Tag::NotExistTag& )
	{
		return OPC_E_INVALIDHANDLE;
	}
	VARIANT tmp;
	::VariantInit( &tmp );
	os::win32::com::Variant::variantCopy( &tmp, &newValue );
	HRESULT result = ::VariantChangeType( &tmp, &tmp, 0, tagRef->getCanonicalDataType() );
	if( FAILED( result) )
		return result;
	tagRef->write( tmp );
	return S_OK;
}

const FILETIME& GroupItem::getTimeStamp() const
{
	return lastChange;
}

DWORD GroupItem::getAccessRights()
{
	if( tagRef == NULL )
		tagRef = opcAddressSpace::getInstance().getLeaf( itemID );

	return tagRef->getAccessRights();
}

WORD GroupItem::getQuality()
{
	if( tagRef == NULL )
		tagRef = opcAddressSpace::getInstance().getLeaf( itemID );

	return tagRef->getQuality();
}

frl::Bool GroupItem::isChange()
{
	if( tagRef == NULL )
		tagRef = opcAddressSpace::getInstance().getLeaf( itemID );

	FILETIME tmp = tagRef->getTimeStamp();
	return ( ( lastChange.dwHighDateTime != tmp.dwHighDateTime)
				|| ( lastChange.dwLowDateTime != tmp.dwLowDateTime ) );
}

GroupItem* GroupItem::clone() const
{
	GroupItem *grItem= new GroupItem();
	grItem->clientHandle = clientHandle;
	grItem->actived = actived;
	grItem->accessPath = accessPath;
	grItem->itemID = itemID;
	grItem->requestDataType = requestDataType;
	return grItem;
}

const os::win32::com::Variant& GroupItem::getCachedValue() const
{
	return cachedValue;
}

void GroupItem::resetTimeStamp()
{
	lastChange.dwLowDateTime = 0;
	lastChange.dwHighDateTime = 0;
}

void GroupItem::setTimeStamp( FILETIME &ts )
{
	if( tagRef == NULL )
		tagRef = opcAddressSpace::getInstance().getLeaf( itemID );
	tagRef->setTimeStamp( ts );
}

void GroupItem::setQuality( WORD quality )
{
	if( tagRef == NULL )
		tagRef = opcAddressSpace::getInstance().getLeaf( itemID );
	tagRef->setQuality( quality );
}

} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
