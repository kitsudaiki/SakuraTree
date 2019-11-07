[test_tree]
packages: nano

{
	[BRANCH: install_branch, HOST: sakura]
	address: "192.168.56.101"
	user: sakura
	port: 22
	ssh_key: "~/.ssh/sakura_test"
	packages: "{{packages}}"
}
