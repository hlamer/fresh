#ifndef PSETTINGS_H
#define PSETTINGS_H

/*!
	\file pSettings.h
	\brief An extended QSettings class that handle ini files with different contexts.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QSettings>

#if defined( QT_GUI_LIB )
class QMainWindow;
#endif

/*!
	\ingroup Core
	\class pSettings
	\brief An extended QSettings class that handle ini files with different contexts.
	
	This class is used for storing/retreiving data using ini files with different contexts (Auto, Normal, Portable).
*/
class FRESH_EXPORT pSettings : public QSettings
{
	Q_OBJECT

public:
	/*! Thie enumeration represent the possible choices for the location of your pSettings object. */
	enum Type {
		Invalid = -1, /*!< An invalid path. */ 
		Auto, /*!< The pSettings decide which enum to use. pSettings::Portable if the binary path is writable, else pSettings::Normal. */ 
		Normal, /*!< The init file will be created in the user home. */ 
		Portable /*!< The ini file will be created in the binary path if possible, else fallback to pSettings::Normal mode. */ 
	};
	
	/*!
		\class pSettings::Properties
		\brief This class defined the properties to apply to a pSettings class.
		\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
	*/
	struct FRESH_EXPORT Properties
	{
		/*!
			\brief A Properties object initialized with default properties
			\param other The properties to copy.
		*/
		Properties( const pSettings::Properties& other = pSettings::defaultProperties() );
		/*!
			\brief A Properties object initialized with custom parameters.
			\param name The name of the application.
			\param version The application version.
			\param type The pSettings::Type type.
		*/
		Properties( const QString& name, const QString& version, pSettings::Type type );
		/*!
			\brief Return the path where is located the ini file when using the pSettings::Normal type.
			\return A QString containing the absolute path of the ini file.
		*/
		QString storageLocation() const;
		/*!
			Return a deducted file path for \a type.
		*/
		QString filePath( pSettings::Type type ) const;
		/*!
			\brief The absolute file path of the ini file.
			\return The absolute file path of the ini file.
		*/
		QString settingsFilePath() const;
		/*!
			\brief Return an absolute file path of a ini file having application name \a name and application version \a version according to the current properties.
			\param name The application name.
			\param version The application version.
			\return The file path of the ini file.
		*/
		QString settingsFilePath( const QString& name, const QString& version ) const;
		/*!
			\brief Return true if \a filePath is writable (ie: file/path) can be created/writed.
			\param filePath The file path to check.
			\return Return true if is writable, else false.
		*/
		bool isWritable( const QString& filePath ) const;
		
		QString name;
		QString version;
		pSettings::Type type;
	};
	/*!
		\brief Create a pSettings object having parent \a parent and properties \a properties.
		\param parent The parent object.
		\param properties The properties to apply to this pSettings.
	*/
	pSettings( QObject* parent = 0, const pSettings::Properties& properties = defaultProperties() );
	/*!
		\brief Create a pSettings object having parent \a parent and properties \a properties with overrided \a name and \a version.
		\param parent The parent object.
		\param name The application name.
		\param version The application version.
		\param properties The properties to apply to this pSettings.
	*/
	pSettings( QObject* parent, const QString& name, const QString& version, const pSettings::Properties& properties = defaultProperties() );
	/*!
		\brief Return the properties applied to this pSettings.
		\return A Properties object.
	*/
	pSettings::Properties properties() const;

#if defined( QT_GUI_LIB )
	/*!
		\brief Restore the state of a QMainWindow.
		\param window The main window to restore state
		\sa QMainWindow::restoreState()
		\note This member is only defined if QT_GUI_LIB is defined.
	*/
	virtual void restoreState( QMainWindow* window );
	/*!
		\brief Save the state of a QMainWindow.
		\param window The window to restore state.
		\sa QMainWindow::saveState()
		\note This member is only defined if QT_GUI_LIB is defined.
	*/
	virtual void saveState( QMainWindow* window );
#endif
	/*!
		\brief Set the default settings.
		
		You can reimplement this member to set default ini settings, by example after the first application run.
	*/
	virtual void setDefaultSettings();
	/*!
		\brief Set the default properties to be used.
		
		Typically this should be defined in the main, just after the initialization of the QCoreApplication / QApplication.
		\param properties The properties to be used as default.
	*/
	static void setDefaultProperties( const pSettings::Properties& properties );
	/*!
		\brief Return the default Properties used when creating pSettings.
		\return A Properties object.
	*/
	static pSettings::Properties defaultProperties();

protected:
	static pSettings::Properties mDefaultProperties;
	pSettings::Properties mProperties;
};

#endif // PSETTINGS_H
