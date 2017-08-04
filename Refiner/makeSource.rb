# -*- encoding: utf-8 -*-
=begin
 ruby Refiner/makeSource.rb
=end

require 'fileutils'

$refiner_version = "1.1.03"
File.open( 'Refiner/VERSION' ){|aFile|
	line = aFile.gets
	$refiner_version = line[/\d\.\d\.\d+/]
}
productName = "REVOCAP_Refiner Ver.#{$refiner_version}"
puts productName

releaseDir = "Release"
date = Time.new.strftime('%Y_%m_%d_%H_%M')
date2 = Time.new.strftime('%Y%m%d')

# Release 用ディレクトリをなければ作る

FileUtils.mkdir_p releaseDir
outputDir = File.join(releaseDir,"REVOCAP_Refiner-#{$refiner_version}")
FileUtils.mkdir_p outputDir

files = []
files.concat Dir.glob("Common/{*.{h,cpp}}")
files.concat Dir.glob("Geometry/{*.{h,cpp}}")
files.concat Dir.glob("Matrix/{*.{h,cpp}}")
files.concat Dir.glob("MeshDB/{*.{h,cpp}}")
files.concat Dir.glob("MeshGen/{*.{h,cpp}}")
files.concat Dir.glob("RevocapIO/{*.{h,cpp}}")
files.concat Dir.glob("Refiner/{*.{h,cpp,c,F90,f,inc}}")
files.concat Dir.glob("Shape/{*.{h,cpp}}")

# OpenCASCADE に依存するコードを除く
files -= Dir.glob(File.join("Shape","kmbCADFileIO.*"))
files -= Dir.glob(File.join("Shape","kmbShapeData.*"))
files -= Dir.glob(File.join("Shape","kmbPatchGenerator.*"))

linecounter = 0

files.each{ |filename|
	basename = File.basename(filename,".*")
	dirname = File.dirname(filename)
	FileUtils.mkdir_p File.join( outputDir, dirname )
	input = File.open( filename,"r")
	output = File.open( File.join(outputDir,filename), "wb" )
	puts filename
	case File.extname( filename )
	when ".cpp",".h"
		# "//" スタイルのコメントは削除する
		# REVOCAP_Debug スタイルのデバッグ出力も削除
		# include kmbDebug.h があれば削除
		debugflag = false
		input.each{ |line|
			linecounter += 1
			line.gsub!(/\/\/.*/,"") if line.index("\"") == nil
			line = "" if line.lstrip.index("//") == 0
			line.rstrip!
			if line =~ /^\s*REVOCAP_Debug/
				debugflag = true
			end
			if debugflag || line =~ /^#include "Common\/kmbDebug.h"/
				puts line
			else
				output.puts line
			end
			if debugflag && line =~ /;\s*$/
				debugflag = false
			end
		}
	else
		input.each{ |line|
			line.rstrip!
			output.puts line
		}
	end
	input.close
	output.close
}

# ディレクトリを変えてコピー

docDir = File.join(outputDir,"Documents")
FileUtils.mkdir_p docDir
FileUtils.cp File.join("Refiner","OPTIONS"), outputDir
FileUtils.cp File.join("Refiner","VERSION"), outputDir
FileUtils.cp File.join("Refiner","README.txt"), outputDir
FileUtils.cp File.join("Refiner","Makefile"), outputDir
FileUtils.cp Dir.glob( File.join("Refiner","Documents","*.pdf") ), docDir

# 単純コピー

copyfiles = []
copyfiles.concat Dir.glob("Refiner/data/*/*")
copyfiles.concat Dir.glob("config/MakefileConfig.*[^~]")
copyfiles.concat Dir.glob("MakefileRefiner.in")
copyfiles.concat Dir.glob("Documents/*.pdf")
copyfiles.concat Dir.glob("Documents/images/*.gif")

copyfiles.each{ |filename|
	basename = File.basename(filename,".*")
	dirname = File.dirname(filename)
	FileUtils.mkdir_p File.join( outputDir, dirname )
	FileUtils.cp filename, File.join( outputDir, dirname )
}

FileUtils.cd releaseDir
system("tar cfvz REVOCAP_Refiner_#{date2}.tar.gz REVOCAP_Refiner-#{$refiner_version}")
FileUtils.cd "REVOCAP_Refiner-#{$refiner_version}"
system("make Refiner")
system("make sample")
system("Refiner/sampleC")
system("Refiner/sampleF")
FileUtils.cd "../.."
FileUtils.mv(outputDir, File.join(releaseDir,date.to_s+"RefinerSrc"))
puts "REVOCAP_Refiner ver. #{$refiner_version}"
puts "line count = #{linecounter}"
