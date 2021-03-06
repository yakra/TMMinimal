CXX = clang++
STD = -std=c++11
CXXFLAGS = -Wno-comment -Wno-dangling-else -Wno-logical-op-parentheses

MTObjects = siteupdateMT.o functions/threads.o \
  classes/WaypointQuadtree/WaypointQuadtreeMT.o

STObjects = siteupdateST.o \
  classes/WaypointQuadtree/WaypointQuadtreeST.o

CommonObjects = \
  classes/Args/Args.o \
  classes/HighwaySystem/HighwaySystem.o \
  classes/Route/Route.o \
  classes/Route/read_wpt.o \
  classes/Waypoint/Waypoint.o \
  functions/crawl_hwy_data.o \
  functions/split.o \
  functions/valid_num_str.o

.PHONY: all clean
all: siteupdate siteupdateST

%MT.d: %.cpp ; $(CXX) $(CXXFLAGS) $(STD) -MM -D threading_enabled $< | sed -r "s~.*:~$*MT.o:~" > $@
%ST.d: %.cpp ; $(CXX) $(CXXFLAGS) $(STD) -MM $< | sed -r "s~.*:~$*ST.o:~" > $@
%.d  : %.cpp ; $(CXX) $(CXXFLAGS) $(STD) -MM $< | sed -r "s~.*:~$*.o:~" > $@

-include $(MTObjects:.o=.d)
-include $(STObjects:.o=.d)
-include $(CommonObjects:.o=.d)

%MT.o: ; $(CXX) $(CXXFLAGS) $(STD) -o $@ -D threading_enabled -c $<
%ST.o: ; $(CXX) $(CXXFLAGS) $(STD) -o $@ -c $<
%.o  : ; $(CXX) $(CXXFLAGS) $(STD) -o $@ -c $<

siteupdate: $(MTObjects) $(CommonObjects)
	@echo Linking siteupdate...
	@$(CXX) $(CXXFLAGS) $(STD) -pthread -o siteupdate $(MTObjects) $(CommonObjects)
siteupdateST: $(STObjects) $(CommonObjects)
	@echo Linking siteupdateST...
	@$(CXX) $(CXXFLAGS) $(STD) -o siteupdateST $(STObjects) $(CommonObjects)

clean:
	@rm -f $(MTObjects)       $(STObjects)       $(CommonObjects)
	@rm -f $(MTObjects:.o=.d) $(STObjects:.o=.d) $(CommonObjects:.o=.d)
