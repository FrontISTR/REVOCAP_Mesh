# -*- coding: utf-8 -*-
=begin
Linux/Windows 共通の拡張ライブラリ作成スクリプト extconf.rb

Linux 環境の場合、ldd RevocapMeshGen.so で確認！
=end

require 'mkmf'
require 'fileutils'

modulename = "RevocapMeshGen"

puts "extconf for #{modulename} #{RUBY_PLATFORM} #{RUBY_VERSION}"

case RUBY_PLATFORM
when "i486-linux", "x86_64-linux", "x86_64-linux-gnu", "i686-linux"
  FileUtils.mkdir_p File.join('..','lib',RUBY_PLATFORM)
#	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
#		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
#	end
	CONFIG['LDSHARED'] = 'g++ -shared -s'
	arch = RUBY_PLATFORM
when "i386-cygwin"
  FileUtils.mkdir_p File.join('..','lib',RUBY_PLATFORM)
#	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
#		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
#	end
	CONFIG['CC'] = 'gcc-4'
	CONFIG['CXX'] = 'g++-4'
	CONFIG['LDSHARED'] = 'g++-4 -shared -s'
	arch = RUBY_PLATFORM
else
  FileUtils.mkdir_p File.join('..','lib',RUBY_PLATFORM)
#	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
#		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
#	end
	arch = RUBY_PLATFORM
end

$libs = append_library($libs, 'RcapCommon')
$libs = append_library($libs, 'RcapMatrix')
$libs = append_library($libs, 'RcapGeometry')
$libs = append_library($libs, 'RcapMeshDB')
$libs = append_library($libs, 'RcapMeshGen')
dir_config( "#{modulename}", "..","../lib/#{arch}")
create_makefile("#{modulename}")    # interface ファイルで定義した module 名を書く
