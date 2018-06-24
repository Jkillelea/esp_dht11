{
    split($0, a, " ");
    split(a[2], b, "/");

    time = a[1];
    val = a[3];
    id = b[1]; 

    print(time " " id " "  val);
}
