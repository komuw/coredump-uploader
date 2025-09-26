package main

import (
	"bytes"
	"errors"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
)

func main() {
	exePath := "../my_seg"
	corePath := "/home/kw/mystuff/coredump-uploader/core.587633.my_seg.1758865156.kw"
	// debugInfoDirs := []string{}

	fmt.Println("======HERE==========")
	if err := callGdb("", corePath, exePath); err != nil {
		panic(err)
	}
}

func callGdb(gdbPath, corePath, exePath string) error {
	if corePath == "" {
		return errors.New("corePath is empty")
	}
	corePath, err := filepath.Abs(corePath)
	if err != nil {
		return fmt.Errorf("cannot find absolute path for corePath: %w", err)
	}

	if exePath == "" {
		// note: we could try and parse out the exePath from the corePath; https://github.com/purecloudlabs/gprovision/blob/b79a0d8da330cf23243c651e8d923e9450eeccde/pkg/corer/backtrace/gdb.go#L39
		return errors.New("exePath is empty")
	}
	exePath, err = filepath.Abs(exePath)
	if err != nil {
		return fmt.Errorf("cannot find absolute path for corePath: %w", err)
	}

	if gdbPath == "" {
		gdbPath = "gdb"
	}
	gdbPath, err = exec.LookPath(gdbPath)
	if err != nil {
		return fmt.Errorf("unable to lookup gdb: %w", err)
	}

	fmt.Println("gdbPath: ", gdbPath)
	fmt.Println("corePath: ", corePath)
	fmt.Println("exePath: ", exePath)

	{
		// gdb --quiet --silent --batch -x commands.gdb my_seg core.587633.my_seg.1758865156.kw
		f, err := os.CreateTemp("", "gdb_commands")
		if err != nil {
			return fmt.Errorf("unable to create gdb_commands file: %w", err)
		}
		defer f.Close()
		defer os.Remove(f.Name())
		if _, err := f.Write([]byte("thread apply all bt\nquit")); err != nil {
			return fmt.Errorf("unable to write to gdb_commands file: %w", err)
		}
		fmt.Println("gdb_commands file: ", f.Name())

		args := []string{gdbPath, "--quiet", "--silent", "--batch", "-x", f.Name(), exePath, corePath}
		cmd := exec.Command(args[0], args[1:]...)
		var stdErr bytes.Buffer
		var stdOut bytes.Buffer
		cmd.Stderr = &stdErr
		cmd.Stdout = &stdOut
		if err := cmd.Run(); err != nil {
			return fmt.Errorf("unable to run gdb(%v): %w", cmd.Args, err)
		}
		if (stdErr.String() != "") || (stdOut.String() == "") {
			return fmt.Errorf("gdb run failed(%v): `%s`", cmd.Args, stdErr.String())
		}

		fmt.Println("stdOut:", stdOut.String())
	}

	return nil
}
