#include "FileSystemUtils.h"

QString FileSystemUtils::findFile( QDir& dir, const QString& fileName, bool recursive )
{
	foreach ( const QFileInfo& fi, dir.entryInfoList( QStringList( fileName ), QDir::Files | QDir::CaseSensitive ) ) {
		if ( fi.fileName() == fileName ) {
			return fi.canonicalFilePath();
		}
	}
	
	if ( recursive ) {
		foreach ( const QFileInfo& fi, dir.entryInfoList( QDir::AllDirs ) ) {
			dir.setPath( fi.absoluteFilePath() );
			const QString fn = findFile( dir, fileName, true );
			
			if ( !fn.isNull() ) {
				return fn;
			}
		}
	}
	
	return QString::null;
}
