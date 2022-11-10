puts "PWD=" + Dir.pwd
puts $:

require 'RevocapShape'

puts RevocapShape::VERSION
puts RevocapShape::COPYRIGHT

require 'test/unit'

class TestRevocapShape < Test::Unit::TestCase

	def setup
	end

	def teardown
	end

	def test_exist_shape
		assert( RevocapShape )
		assert_instance_of( Module, RevocapShape )
		assert_instance_of( Class, RevocapShape::ShapeData )
		assert_instance_of( Class, RevocapShape::PatchGenerator )
		assert_instance_of( Class, RevocapShape::CADFileIO )
		shape = RevocapShape::ShapeData.new
		assert_instance_of( RevocapShape::ShapeData, shape )
	end
end
