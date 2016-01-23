# -*- encoding: utf-8 -*-
=begin
Linux/Windows 共通の拡張ライブラリ作成スクリプト extconf.rb

Linux 環境の場合、ldd RevocapGL.so で確認すること
OpenCASCADE の include と lib の場所を変更する必要があるかもしれない
=end

require 'mkmf'

modulename = "RevocapShape"

use_opencascade = (ENV['OPENCASCADEFLAG'] != "")

casroot = ENV['CASROOT'] || "/usr/local/opencascade"

if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
	Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
end
arch = RUBY_PLATFORM

case RUBY_PLATFORM
when /linux/
	dir_config( "#{modulename}", "/usr/include/opencascade", "/usr/lib" )
	CONFIG['LDSHARED'] = 'g++ -shared -s'
when /mingw/
	dir_config( "#{modulename}", "#{casroot}/include/oce", "#{casroot}/Win64/lib" )
	CONFIG['LDSHARED'] = 'g++ -shared -s'
when /cygwin/
	CONFIG['LDSHARED'] = 'g++ -shared -s'
	use_opencascade = false
else
	use_opencascade = false
end

puts "ARCH = #{arch}"
puts "OPENCASCADE = #{use_opencascade}"
if use_opencascade
[
'TKSTEPBase',
'TKSTEPAttr',
'TKSTEP209',
'TKSTEP',
'TKSTL',
'TKVRML',
'TKMesh',
'TKTopAlgo',
'TKService',
'TKPShape',
'TKPrim',
'TKPCAF',
'TKPLCAF',
'TKOffset',
'TKMath',
'TKHLR',
'TKGeomBase',
'TKGeomAlgo',
'TKG3d',
'TKG2d',
'TKFillet',
'TKFeat',
'TKernel',
'TKCDF',
'TKCAF',
'TKLCAF',
'TKXCAF',
'TKBool',
'PTKernel',
'FWOSPlugin',
'TKBO',
'TKShapeSchema',
'TKXSBase',
'TKShHealing',
'TKBRep',
'TKIGES',
'TKXDEIGES',
'TKBinL'
].each{ |libname|
	$libs = append_library($libs, libname)
}
end

$libs = append_library($libs, 'RcapCommon')
$libs = append_library($libs, 'RcapMatrix')
$libs = append_library($libs, 'RcapGeometry')
$libs = append_library($libs, 'RcapMeshDB')
$libs = append_library($libs, 'RcapShape')
dir_config( "#{modulename}", "..","../lib/#{arch}")
create_makefile("#{modulename}")    # interface ファイルで定義した module 名を書く
