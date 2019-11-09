[test_tree]
- packages: nano

{
	seed(sakura)
	- address: "192.168.56.101"
	- port: 1337
	- ssh_user: sakura
	- ssh_port: 22
	- ssh_key: "~/.ssh/sakura_test" 
	{
		branch(install_branch)
		- packages: "{{packages}}"
	}
	
}
