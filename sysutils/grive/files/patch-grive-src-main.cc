--- grive/src/main.cc.orig	2013-08-03 17:39:31.303937000 -0500
+++ grive/src/main.cc	2013-08-03 18:13:57.085937744 -0500
@@ -66,35 +66,35 @@
 
 void InitLog( const po::variables_map& vm )
 {
-	std::auto_ptr<log::CompositeLog> comp_log(new log::CompositeLog) ;
-	LogBase* console_log = comp_log->Add( std::auto_ptr<LogBase>( new log::DefaultLog ) ) ;
+	std::auto_ptr<gr::log::CompositeLog> comp_log(new gr::log::CompositeLog) ;
+	LogBase* console_log = comp_log->Add( std::auto_ptr<LogBase>( new gr::log::DefaultLog ) ) ;
 
 	if ( vm.count( "log" ) )
 	{
-		std::auto_ptr<LogBase> file_log(new log::DefaultLog( vm["log"].as<std::string>() )) ;
-		file_log->Enable( log::debug ) ;
-		file_log->Enable( log::verbose ) ;
-		file_log->Enable( log::info ) ;
-		file_log->Enable( log::warning ) ;
-		file_log->Enable( log::error ) ;
-		file_log->Enable( log::critical ) ;
+		std::auto_ptr<LogBase> file_log(new gr::log::DefaultLog( vm["log"].as<std::string>() )) ;
+		file_log->Enable( gr::log::debug ) ;
+		file_log->Enable( gr::log::verbose ) ;
+		file_log->Enable( gr::log::info ) ;
+		file_log->Enable( gr::log::warning ) ;
+		file_log->Enable( gr::log::error ) ;
+		file_log->Enable( gr::log::critical ) ;
 		
 		// log grive version to log file
-		file_log->Log( log::Fmt("grive version " VERSION " " __DATE__ " " __TIME__), log::verbose ) ;
-		file_log->Log( log::Fmt("current time: %1%") % DateTime::Now(), log::verbose ) ;
+		file_log->Log( gr::log::Fmt("grive version " VERSION " " __DATE__ " " __TIME__), gr::log::verbose ) ;
+		file_log->Log( gr::log::Fmt("current time: %1%") % DateTime::Now(), gr::log::verbose ) ;
 		
 		comp_log->Add( file_log ) ;
 	}
 	
 	if ( vm.count( "verbose" ) )
 	{
-		console_log->Enable( log::verbose ) ;
+		console_log->Enable( gr::log::verbose ) ;
 	}
 	
 	if ( vm.count( "debug" ) )
 	{
-		console_log->Enable( log::verbose ) ;
-		console_log->Enable( log::debug ) ;
+		console_log->Enable( gr::log::verbose ) ;
+		console_log->Enable( gr::log::debug ) ;
 	}
 	LogBase::Inst( std::auto_ptr<LogBase>(comp_log.release()) ) ;
 }
@@ -143,7 +143,7 @@
 	
 	Config config(vm) ;
 	
-	Log( "config file name %1%", config.Filename(), log::verbose );
+	Log( "config file name %1%", config.Filename(), gr::log::verbose );
 
 	if ( vm.count( "auth" ) )
 	{
@@ -177,7 +177,7 @@
 		Log(
 			"Please run grive with the \"-a\" option if this is the "
 			"first time you're accessing your Google Drive!",
-			log::critical ) ;
+			gr::log::critical ) ;
 		
 		return -1;
 	}
@@ -197,7 +197,7 @@
 		drive.DryRun() ;
 	
 	config.Save() ;
-	Log( "Finished!", log::info ) ;
+	Log( "Finished!", gr::log::info ) ;
 	return 0 ;
 }
 
@@ -209,15 +209,15 @@
 	}
 	catch ( Exception& e )
 	{
-		Log( "exception: %1%", boost::diagnostic_information(e), log::critical ) ;
+		Log( "exception: %1%", boost::diagnostic_information(e), gr::log::critical ) ;
 	}
 	catch ( std::exception& e )
 	{
-		Log( "exception: %1%", e.what(), log::critical ) ;
+		Log( "exception: %1%", e.what(), gr::log::critical ) ;
 	}
 	catch ( ... )
 	{
-		Log( "unexpected exception", log::critical ) ;
+		Log( "unexpected exception", gr::log::critical ) ;
 	}
 	return -1 ;
 }
