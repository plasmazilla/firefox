/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/  */

function run_test() {
  var nsILocalFile = Components.interfaces.nsILocalFile;
  var root = Components.classes["@mozilla.org/file/local;1"].
              createInstance(nsILocalFile);

  // copied from http://mxr.mozilla.org/mozilla-central/source/modules/libpr0n/test/unit/test_imgtools.js#135
  // nsIXULRuntime.OS doesn't seem to be available in xpcshell, so we'll use
  // this as a kludgy way to figure out if we're running on Windows.
  var isWindows = ("@mozilla.org/windows-registry-key;1" in Components.classes);
  if (isWindows) {
    root.initWithPath("\\\\.");
  } else {
    root.initWithPath("/");
  }
  var drives = root.directoryEntries;
  do_check_true(drives.hasMoreElements());
  while (drives.hasMoreElements()) {
    var newPath = drives.getNext().QueryInterface(nsILocalFile).path;
    do_check_eq(newPath.indexOf("\0"), -1);
  }
}
