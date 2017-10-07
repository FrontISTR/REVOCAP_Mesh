# -*- encoding: utf-8 -*-
=begin
CISS 公開用コメントを除いたソース生成用のスクリプト

引数
Refiner : REVOCAP_Refiner のソースコード
RefinerCombine : REVOCAP_Refiner のソースコードを一つにまとめる

=end

require 'fileutils'

$revoVer = "1.6.06"
File.open( 'VERSION' ){|aFile|
	line = aFile.gets
	$revoVer = line[/\d\.\d\.\d+/]
}
puts "REVOCAP_Mesh ver. #{$revoVer}"

$refinerVer = "1.1.03"
File.open( 'Refiner/VERSION' ){|aFile|
	line = aFile.gets
	$refinerVer = line[/\d\.\d\.\d+/]
}
puts "REVOCAP_Refiner ver. #{$refinerVer}"

releaseDir = "Release"
productName = "REVOCAP_PrePost Ver.#{$revoVer}"
date = Time.new.strftime('%Y%m%d%H%M')
date2 = Time.new.strftime('%Y%m%d')

# Release 用ディレクトリをなければ作る

FileUtils.mkdir_p releaseDir

### ソースファイルのコピー ###

case ARGV[0]
when "Refiner", "RefinerCombine"
	outputDir = File.join(releaseDir,"REVOCAP_Refiner-#{$refinerVer}")
	FileUtils.mkdir_p outputDir
	files = "{Common,Matrix,Geometry,MeshDB,MeshGen,RevocapIO,Shape}/{*.{h,cpp},Makefile}"
	files << "\0Refiner/{*.{h,cpp,c,F90,f,inc},Makefile}"
	FileUtils.cp File.join("Refiner","README.txt"), File.join(outputDir,"README.txt")
	input = File.open( "Refiner/Makefile","r")
	output = File.open( File.join(outputDir,"Makefile"), "wb")
	inflag = false
	input.each{ |line|
		line.chop!
		if line =~ /^\#?include .+$/
			if !inflag
				output.puts "include OPTIONS"
				output.puts "include MakefileConfig.in"
				inflag = true
			end
		else
			output.puts line
		end
	}
	input.close
	output.close
	FileUtils.cp "MakefileRefiner.in", outputDir
	Dir.glob("MakefileConfig.*[^~]"){|conffile|
		FileUtils.cp conffile, outputDir
	}
	FileUtils.cp File.join("Refiner","OPTIONS"), File.join(outputDir,"OPTIONS")
	FileUtils.cp File.join("Refiner","VERSION"), File.join(outputDir,"VERSION")
else
	outputDir = File.join(releaseDir,"REVOCAP_Mesh-#{$revoVer}")
	FileUtils.mkdir_p outputDir
	files = "{Common,Matrix,Geometry,MeshDB,MeshGen,MeshGL,RevocapIO,Shape}/{*.{h,cpp},Makefile}"
	files << "\0{MeshDBRuby,MeshGenRuby,MeshGLRuby,RevocapIORuby,ShapeRuby}/{*.{i,h},extconf.rb,MakefileSwig}"
	FileUtils.cp "README.txt", outputDir
	input = File.open( "Makefile","r")
	output = File.open( File.join(outputDir,"Makefile"), "wb")
	inflag = false
	input.each{ |line|
		line.chop!
		if line =~ /^\#?include .+$/
			if !inflag
				output.puts "include OPTIONS"
				output.puts "include MakefileRuby.in"
				output.puts "include MakefileConfig.in"
				inflag = true
			end
		else
			output.puts line
		end
	}
	input.close
	output.close
	FileUtils.cp "MakefileRuby.in", outputDir
	Dir.glob("MakefileConfig.*[^~]"){|conffile|
		if conffile !~ /MakefileConfig.as.*/
			FileUtils.cp conffile, outputDir
		end
	}
	FileUtils.cp "OPTIONS", outputDir
	FileUtils.cp "VERSION", outputDir
end

linecounter = 0

combine = nil
if ARGV[0] == "RefinerCombine"
	combine = File.open( File.join(outputDir,"AllSource.txt"),"w" )
end

Dir.glob(files){ |filename|
	next if deleteFileList.include?(filename)
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
			line.force_encoding('Shift_JIS')
			line.gsub!(/\/\/.*/,"")
			line.rstrip!
			if line =~ /^\s*REVOCAP_Debug/
				debugflag = true
			end
			if debugflag || line =~ /^#include "Common\/kmbDebug.h"/
				puts line
			else
				output.puts line
				combine.puts line if combine
			end
			if debugflag && line =~ /;\s*$/
				debugflag = false
			end
		}
	when ".i"
		# "//" スタイルのコメントは削除する
		input.each{ |line|
			linecounter += 1
			line.force_encoding('Shift_JIS')
			line.gsub!(/\/\/.*/,"")
			line.rstrip!
			output.puts line
			combine.puts line if combine
		}
	when ".rb"
		input.each{ |line|
			line.force_encoding('UTF-8')
			line.rstrip!
			output.puts line
		}
	else
		input.each{ |line|
			line.force_encoding('Shift_JIS')
			line.rstrip!
			output.puts line
		}
	end
	input.close
	output.close
}

if combine
	combine.close
end

### データファイルのコピー ###

files = "Documents/*.pdf\0Documents/images/*.gif"

FileUtils.mkdir_p File.join( outputDir, "Documents", "images" )
Dir.glob(files){ |filename|
	next if File.directory?(filename)
	dirname = File.dirname(filename)
	FileUtils.mkdir_p File.join( outputDir, dirname )
	FileUtils.cp filename, File.join( outputDir, dirname )
}

if ARGV[0] == "VC"
	files = "*/*.vcproj"
	Dir.glob(files){ |filename|
		next if File.directory?(filename)
		dirname = File.dirname(filename)
		FileUtils.mkdir_p File.join( outputDir, dirname )
		FileUtils.cp filename, File.join( outputDir, dirname )
	}
elsif ARGV[0] == "Refiner"
	FileUtils.mkdir_p File.join( outputDir, "Documents" )
	Dir.glob("Refiner/Documents/*.pdf"){|filename|
		FileUtils.cp filename, File.join(outputDir,"Documents")
	}
	FileUtils.mkdir_p File.join(outputDir,"Refiner","data")
	Dir.glob("Refiner/data/*/*"){|filename|
		dirname = File.dirname(filename)
		FileUtils.mkdir_p File.join( outputDir, dirname )
		FileUtils.cp filename, File.join(outputDir,dirname)
	}
end

case ARGV[0]
when "Refiner"
	FileUtils.cd releaseDir
	system("tar cfvz REVOCAP_Refiner_#{date2}.tgz REVOCAP_Refiner-#{$refinerVer}")
	FileUtils.cd ".."
	FileUtils.mv(outputDir, File.join(releaseDir,date.to_s+"RefinerSrc"))
	puts "REVOCAP_Refiner ver. #{$refinerVer}"
when "RefinerCombine"
	FileUtils.mv(outputDir, File.join(releaseDir,date.to_s+"RefinerSrc"))
	puts "REVOCAP_Refiner ver. #{$refinerVer}"
else
	FileUtils.cd releaseDir
	system("tar cfvz REVOCAP_Mesh_#{date2}.tgz REVOCAP_Mesh-#{$revoVer}")
	FileUtils.cd ".."
	FileUtils.mv(outputDir, File.join(releaseDir,date.to_s+"Src"))
	puts "REVOCAP_Mesh ver. #{$revoVer}"
end

puts "line count = #{linecounter}"

__END__
