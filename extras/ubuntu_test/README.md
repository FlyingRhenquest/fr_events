# Barebones Dev Setup for fr_events

I put this together to address [Issue 2](https://github.com/FlyingRhenquest/fr_events/issues/2), to make sure that given a barebones system, the script installs all the packages it needs. This uses the base docker image for ubuntu 22.04, which has more or less nothing installed on it. This should work for most ubuntu-derived systems (Ubuntu, Mint and other Ubuntu deratives.) I think it'll work for Debian too but haven't tested that.

To set up the dev environment on any ubuntu-derived linux dist, the
ubuntish.sh script in this directory should install all the necessary packages.

There are two scripts and a dockerfile in this directory. If you want to use the dockerfile, you'll need to [install docker,](https://linuxiac.com/how-to-install-docker-on-linux-mint-21/) which is pretty straightforward on Mint. If you're using a different OS, you'll want to dig around for installation instructions for it. I've never found it to be difficult on any of the operating systems I've used it on. Once you have docker, you can run the command line commands in this directory:

```
 docker build . -t ubuntu_test
 docker run --rm -it --entrypoint bash ubuntu_test
```

This should get you a command prompt in your ubuntu VM. *Note that
anything you run in this VM will go away when you exit it.* There are a number of ways you can modify the dockerfile to be able to make permanent changes which are beyond the scope of this document.

This installs the dev environment:

```
/tmp/ubuntish.sh
```

The next script creates a ~/sandbox and clones this repo into
that directory, creates a ~/build/fr_events directory and
builds the library and python API into that directory. Note
that this will take a few minutes. When it's done, it runs
the unit tests and the python example from that directory.

```
/tmp/build_events.sh
```
