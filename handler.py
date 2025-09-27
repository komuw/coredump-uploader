def main():
    import sys
    print("====args======")
    print(sys.argv)
    log_file = "/tmp/coredump_log.txt"
    with open(log_file, "a") as f:
        f.write(f"Received arguments: {str(sys.argv)}\n\n")
    print("====args======")

if __name__ == "__main__":
    main()
