# -*- coding: utf-8 -*-
=begin
Linux/Windows 共通の拡張ライブラリ作成スクリプト extconf.rb

Linux 環境の場合、ldd RevocapGL.so で確認！
=end

require 'mkmf'

modulename = "RevocapGL"

case RUBY_PLATFORM
when "i486-linux", "x86_64-linux", "x86_64-linux-gnu", "i686-linux"
	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
	end
	CONFIG['LDSHARED'] = 'g++ -shared -s'
	arch = RUBY_PLATFORM
	$libs = append_library($libs, 'GL')
	$libs = append_library($libs, 'GLU')
	$libs = append_library($libs, 'GLEW')
when "i386-cygwin"
	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
	end
	CONFIG['CC'] = 'gcc-4'
	CONFIG['CXX'] = 'g++-4'
	CONFIG['LDSHARED'] = 'g++-4 -shared -s'
	arch = RUBY_PLATFORM
	$libs = append_library($libs, 'GL')
	$libs = append_library($libs, 'GLU')
	$libs = append_library($libs, 'GLEW')
when "x64-mingw32"
	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
	end
	arch = RUBY_PLATFORM
	$libs = append_library($libs, 'opengl32')
	$libs = append_library($libs, 'glu32')
	$libs = append_library($libs, 'glew32')
	dir_config( "#{modulename}", "/include", "/lib")
	dir_config( "#{modulename}", "/local/include", "/local/lib")
else
	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
	end
	arch = RUBY_PLATFORM
	$libs = append_library($libs, 'GLUT')
	dir_config( "#{modulename}", "/opt/local")
end

$libs = append_library($libs, 'RcapCommon')
$libs = append_library($libs, 'RcapMatrix')
$libs = append_library($libs, 'RcapGeometry')
$libs = append_library($libs, 'RcapMeshDB')
$libs = append_library($libs, 'RcapMeshGL')

dir_config( "#{modulename}", "..","../lib/#{arch}")
create_makefile("#{modulename}")    # interface ファイルで定義した module 名を書く
