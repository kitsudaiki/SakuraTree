[test_tree]
- packages: nano

{
	seed(sakura)
	- address: "127.0.0.1"
	- port: 1337
	- ssh_user: neptune
	- ssh_port: 22
	- ssh_key: "~/.ssh/sakura_test" 
	{
		branch(install_branch)
		- packages: "{{packages}}"
	}
	
}
