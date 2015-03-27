# -*- coding: utf-8 -*-
=begin
Linux/Windows 共通の拡張ライブラリ作成スクリプト extconf.rb

Linux 環境の場合、ldd RevocapMesh.so で確認！

注意：cygwin 環境で gcc ver4 を使う場合は、Makefile に CXX = g++-4 の追記が必要
=end

require 'mkmf'

modulename = " RevocapMesh"

puts "extconf for #{modulename} #{RUBY_PLATFORM} #{RUBY_VERSION}"

case RUBY_PLATFORM
when "i486-linux", "x86_64-linux", "x86_64-linux-gnu", "i686-linux"
	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
	end
	CONFIG['LDSHARED'] = 'g++ -shared -s'
	arch = RUBY_PLATFORM
when "i386-cygwin"
	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
	end
	CONFIG['CC'] = 'gcc-4'
	CONFIG['CXX'] = 'g++-4'
	CONFIG['LDSHARED'] = 'g++-4 -shared -s'
	arch = RUBY_PLATFORM
else
	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
	end
	arch = RUBY_PLATFORM
end

$libs = append_library($libs, 'RcapCommon')
$libs = append_library($libs, 'RcapMatrix')
$libs = append_library($libs, 'RcapGeometry')
$libs = append_library($libs, 'RcapMeshDB')
dir_config( "#{modulename}", '..', "../lib/#{arch}")
create_makefile("#{modulename}")    # interface ファイルで定義した module 名を書く
