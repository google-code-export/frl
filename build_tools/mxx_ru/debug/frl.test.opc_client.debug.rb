require 'mxx_ru/cpp'
require '../template/frl.test.template'

MxxRu::Cpp::exe_target("frl.test.opc_client.debug.rb")\
{
	required_prj( "frl.lib.debug.rb" )
	test_setup()

	target("test_opc_client_d")
	include_path("../../../test/opc_client")
	runtime_mode( MxxRu::Cpp::RUNTIME_DEBUG )
	rtl_mode( MxxRu::Cpp::RTL_STATIC )
	threading_mode( MxxRu::Cpp::THREADING_MULTI )
	obj_placement( MxxRu::Cpp::CustomSubdirObjPlacement.new( "../../../output/test/opc_client",\
	"../../../output/test/opc_client/obj/#{mxx_runtime_mode}/1/2/3" ) )
	cpp_sources Dir.glob( "../../../test/opc_client/**/*.cpp" )
}