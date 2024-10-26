add_rules("mode.debug", "mode.release")
add_requires("gtest", { configs = { main = true } })

target("leftist-tree", function()
	add_packages("gtest")
	add_includedirs("include")
	add_files("test/leftist-tree.cpp")
end)
