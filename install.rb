$filepath = File.dirname($0)
if !$:.include?($filepath)
	$: << $filepath
end

require 'rbconfig'
require 'fileutils'

module RevocapMesh
class Installer
	def initialize
		@installfiles = [
			'RevocapMesh.so',
			'RevocapGL.so',
			'RevocapMeshGen.so'
			'RevocapIO.so'
			'RevocapShape.so'
		]
	end
	def install
		sitearchdir = Config::CONFIG["sitearchdir"]
		FileUtils.mkdir_p sitearchdir, :mode => 0755
		@installfiles.each{ |f|
			FileUtils.install File.join('bin',f), File.join(sitearchdir,f), :mode => 0755
			puts "installed #{f}"
		}
	end
	def uninstall
		sitearchdir = Config::CONFIG["sitearchdir"]
		@installfiles.each{ |f|
			FileUtils.rm_rf File.join(sitearchdir,f)
			puts "remove #{f}"
		}
	end
end
end

if $0 == __FILE__ then
	begin
		installer = RevocapMesh::Installer.new
		installer.send( ARGV[0] || "install" )
	rescue => err
		raise if $DEBUG
		$stderr.puts err.message
		exit 1
	end
end
