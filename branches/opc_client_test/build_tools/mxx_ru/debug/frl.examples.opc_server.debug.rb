require 'mxx_ru/cpp'

MxxRu::Cpp::exe_target("frl.examples.opc_server.debug.rb")\
{
	required_prj( "frl.lib.debug.rb" )
	target("exaple_opc_server_d")
	include_path("../../../examples/opc_server")
	runtime_mode( MxxRu::Cpp::RUNTIME_DEBUG )
	rtl_mode( MxxRu::Cpp::RTL_STATIC )
	threading_mode( MxxRu::Cpp::THREADING_MULTI )
	obj_placement( MxxRu::Cpp::CustomSubdirObjPlacement.new( "../../../output/examples/opc_server",\
	"../../../output/examples/opc_server/obj/#{mxx_runtime_mode}/1/2/3" ) )
	cpp_sources Dir.glob( "../../../examples/opc_server/**/*.cpp" )
}
