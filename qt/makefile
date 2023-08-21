all: dash-qt

dash-qt:
	mkdir -p ../qt_build
	cd ../qt_build && qmake ../qt/dash_qt.pro
	make -C ../qt_build
	mv ../qt_build/dash_qt ../qt_build/dash-qt
	cp ../qt_build/dash-qt ../bin/dash-qt