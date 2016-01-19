#
# oce の自動ビルドスクリプト
# 階層の深いパスではパスの長さの制限のためエラーになる
#
require 'net/http'
require 'net/https'
require 'fileutils'

def wget(location,path=nil)
  puts "wget #{location}"
  uri = URI(location)
  if path == nil
    path = File.basename(uri.path)
  end
  case uri.scheme
  when 'http'
    http = Net::HTTP.new(uri.host)
    http.start {|w|
      response = w.get(uri.path)
      puts response
      case response
      when Net::HTTPSuccess
        File.open(path,"wb"){|aFile|
          aFile.write response.body
        }
      when Net::HTTPRedirection
        redirect = response['location']
        wget(redirect,path)
      end
    }
  when 'https'
    https = Net::HTTP.new(uri.host, 443)
    https.use_ssl = true
    https.verify_mode = OpenSSL::SSL::VERIFY_NONE
    https.verify_depth = 5
    https.start {|w|
      response = w.get(uri.path)
      puts response
      case response
      when Net::HTTPSuccess
        File.open(path,"wb"){|aFile|
          aFile.write response.body
        }
      when Net::HTTPRedirection
        redirect = response['location']
        wget(redirect,path)
      end
    }
  end
end
=begin
# cmake
if !File.exists?('cmake-3.4.1.tar.gz')
  wget('https://cmake.org/files/v3.4/cmake-3.4.1.tar.gz')
end
if !File.exists?('cmake-3.4.1')
  system('tar xvfz cmake-3.4.1.tar.gz')
end
FileUtils.chdir("cmake-3.4.1"){
  puts "cmake-3.4.1"
  system("sh ./configure 2>&1 | tee ../cmake_configure.log")
  system("make 2>&1 | tee ../cmake_make.log")
}

# zlib
if !File.exist?('zlib-1.2.8.tar.gz')
  wget('http://sourceforge.net/projects/libpng/files/zlib/1.2.8/zlib-1.2.8.tar.gz')
end
if !File.exists?("zlib-1.2.8")
  system('tar xvfz zlib-1.2.8.tar.gz')
end
FileUtils.chdir("zlib-1.2.8"){
  puts "zlib-1.2.8"
  system('make -fwin32/Makefile.gcc 2>&1 | tee ../zlib_make.log')
}
=end
# oce
if !File.exist?('master.zip')
  wget('https://github.com/tpaviot/oce/archive/master.zip')
end
if !File.exists?("oce-master")
  system('unzip master.zip')
end
FileUtils.mkdir_p("build")
FileUtils.chdir("build"){
  system('pwd')
  system('../cmake-3.4.1/bin/cmake -G "MSYS Makefiles" ' +
         '-DOCE_BUILD_TYPE="Release" ' +
         '-DOCE_DRAW=OFF ' +
         '-DOCE_VISUALISATION=OFF ' +
         '-DOCE_WITH_GL2PS=OFF ' +
#         '-DGL2PS_INCLUDE_DIR=/usr/local/include ' +
#         '-DGL2PS_LIBRARY=/usr/local/lib/libgl2ps.a ' +
         '-DOCE_WITH_FREEIMAGE=OFF ' +
#         '-DCMAKE_CXX_FLAGS=-DFREEIMAGE_LIB '
#         '-DFREEIMAGE_INCLUDE_DIR=/usr/local/include ' +
#         '-DFREEIMAGE_LIBRARY=/usr/local/lib/libFreeImage.a ' +
#         '-DFREEIMAGEPLUS_LIBRARY=/usr/local/lib/libfreeimageplus.a ' +
#         '-DFREETYPE_INCLUDE_DIR_freetype2=/usr/local/include/freetype2 ' +
#         '-DFREETYPE_INCLUDE_DIR_ft2build=/usr/local/include ' +
#         '-DFREETYPE_LIBRARY="/usr/local/lib/libfreetype.a;/usr/local/lib/libpng16.a;/usr/local/lib/libz.a" ' +
         '-DOCE_MULTITHREAD_LIBRARY=OPENMP ' +
         '-DOPENMP_LIBRARY=-lgomp ' +
#         '-DOCE_TESTING=ON ' +
         '../oce-master')
  system('make 2>&1 | tee ../oce_make.log')
}
