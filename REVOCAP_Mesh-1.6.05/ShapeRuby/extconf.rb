# -*- encoding: utf-8 -*-
=begin
Linux/Windows 共通の拡張ライブラリ作成スクリプト extconf.rb

Linux 環境の場合、ldd RevocapGL.so で確認すること
OpenCASCADE の include と lib の場所を変更する必要があるかもしれない
=end

require 'mkmf'

modulename = "RevocapShape"

use_opencascade = (ENV['OPENCASCADEFLAG'] != nil)

case RUBY_PLATFORM
when "i486-linux", "x86_64-linux", "x86_64-linux-gnu", "i686-linux"
	dir_config( "#{modulename}", "/usr/include/opencascade", "/usr/lib" )
	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
	end
	CONFIG['LDSHARED'] = 'g++ -shared -s'
	arch = RUBY_PLATFORM
when "i386-cygwin"
	if !File.exist?( File.join('..','lib','i386-cygwin') )
		Dir.mkdir( File.join('..','lib','i386-cygwin') )
	end
	CONFIG['LDSHARED'] = 'g++ -shared -s'
	arch = "i386-cygwin"
	use_opencascade = false
else
	if !File.exist?( File.join('..','lib',RUBY_PLATFORM) )
		Dir.mkdir( File.join('..','lib',RUBY_PLATFORM) )
	end
	arch = RUBY_PLATFORM
	use_opencascade = false
end

if use_opencascade
[
'TKSTEPBase',
'TKSTEPAttr',
'TKSTEP209',
'TKSTEP',
'TKSTL',
'TKVRML',
'TKV3d',
'TKV2d',
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
#'TKDraw', # comment for OpenCASCADE 6.3
'TKCDF',
'TKCAF',
#'TKDCAF', # comment for OpenCASCADE 6.3
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
