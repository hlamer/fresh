/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pMacHelpers.mm
** Date      : 2011-02-20T00:44:21
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#import "pMacHelpers.h"

#include <QDebug>

QVariantMap pMacHelpers::toQVariantMap( CFDictionaryRef dict )
{
	QVariantMap map;
	
	if ( dict ) {
		const CFIndex count = CFDictionaryGetCount( dict );
		const void* keys[ count ];
		const void* values[ count ];
		
		CFDictionaryGetKeysAndValues( dict, keys, values );
		
		for ( CFIndex i = 0; i < count; i++ ) {
			const QVariant key = toQVariant( (CFTypeRef)keys[ i ] );
			const QVariant value = toQVariant( (CFTypeRef)values[ i ] );
			
			map[ key.toString() ] = value;
		}
	}
	
	return map;
}

QVariantList pMacHelpers::toQVariantList( CFArrayRef array )
{
	QVariantList list;
	
	if ( array ) {
		const CFIndex count = CFArrayGetCount( array );
		
		for ( CFIndex i = 0; i < count; i++ ) {
			list << toQVariant( CFArrayGetValueAtIndex( array, i ) );
		}
	}
	
	return list;
}

QVariant pMacHelpers::toQVariant( CFStringRef string )
{
	if ( string ) {
		const CFIndex length = 2 *( CFStringGetLength( string ) +1 ); // Worst case for UTF8
		char buffer[ length ];
		
		if ( CFStringGetCString( string, buffer, length, kCFStringEncodingUTF8 ) ) {
			return QString::fromUtf8( buffer );
		}
		else {
			qWarning() << Q_FUNC_INFO << "CFStringRef conversion failed";
		}
	}
	
	return QVariant();
}

QVariant pMacHelpers::toQVariant( CFBooleanRef value )
{
	return value ? (bool)CFBooleanGetValue( value ) : QVariant();
}

QVariant pMacHelpers::toQVariant( CFNumberRef number )
{
	switch ( CFNumberGetType( number ) ) {
		case kCFNumberSInt8Type:
		case kCFNumberSInt16Type:
		case kCFNumberSInt32Type:
		case kCFNumberSInt64Type:
		case kCFNumberCharType:
		case kCFNumberShortType:
		case kCFNumberIntType:
		case kCFNumberLongType:
		case kCFNumberLongLongType:
		case kCFNumberCFIndexType:
		case kCFNumberNSIntegerType: {
			qint64 value = 0;
			if ( CFNumberGetValue( number, kCFNumberSInt64Type, &value ) ) {
				return value;
			}
			break;
		}
		case kCFNumberFloat32Type:
		case kCFNumberFloat64Type:
		case kCFNumberFloatType:
		case kCFNumberDoubleType:
		case kCFNumberCGFloatType: {
			qreal value = 0;
			if ( CFNumberGetValue( number, kCFNumberFloat64Type, &value ) ) {
				return value;
			}
			break;
		}
	}
	
	return QVariant();
}

QVariant pMacHelpers::toQVariant( CFDataRef data )
{
	return data ? QByteArray( (const char*)CFDataGetBytePtr( data ), CFDataGetLength( data ) ) : QVariant();
}

QVariant pMacHelpers::toQVariant( CFURLRef url )
{
	if ( url ) {
		const CFStringRef string = CFURLCopyFileSystemPath( url, kCFURLPOSIXPathStyle );
		const QVariant variant = toQVariant( string );
		CFRelease( string );
		return variant;
	}
	
	return QVariant();
}

QVariant pMacHelpers::toQVariant( CFUUIDRef uuid )
{
	if ( uuid ) {
		const CFStringRef string = CFUUIDCreateString( kCFAllocatorDefault, uuid );
		const QVariant variant = toQVariant( string );
		CFRelease( string );
		return variant;
	}
	
	return QVariant();
}

QVariant pMacHelpers::toQVariant( CFBundleRef bundle )
{
	return bundle ? toQVariant( CFBundleGetIdentifier( bundle ) ) : QVariant();
}

QVariant pMacHelpers::toQVariant( CFTypeRef ref )
{
	const CFTypeID id = CFGetTypeID( ref );
	
	if ( id == CFStringGetTypeID() ) {
		return toQVariant( (CFStringRef)ref );
	}
	else if ( id == CFBooleanGetTypeID() ) {
		return toQVariant( (CFBooleanRef)ref );
	}
	else if ( id == CFBundleGetTypeID() ) {
		return toQVariant( (CFBundleRef)ref );
	}
	else if ( id == CFNumberGetTypeID() ) {
		return toQVariant( (CFNumberRef)ref );
	}
	else if ( id == CFDictionaryGetTypeID() ) {
		return toQVariantMap( (CFDictionaryRef)ref );
	}
	else if ( id == CFArrayGetTypeID() ) {
		return toQVariantList( (CFArrayRef)ref );
	}
	else if ( id == CFDataGetTypeID() ) {
		return toQVariant( (CFDataRef)ref );
	}
	else if ( id == CFURLGetTypeID() ) {
		return toQVariant( (CFURLRef)ref );
	}
	else if ( id == CFUUIDGetTypeID() ) {
		return toQVariant( (CFUUIDRef)ref );
	}
	
	qWarning() << Q_FUNC_INFO << "Unknow ID" << id;
	CFShow( ref );
	
	return QVariant();
}
