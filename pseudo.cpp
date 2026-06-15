Procedure MulaiDanDistribusiProduksi()
Declaration:
    jumlahWafer, i, sp: Integer
    w: Wafer
Algorithm:
    Input(jumlahWafer)
    If jumlahWafer <= 0 Then
        Output("Jumlah wafer harus lebih dari 0")
        Exit
    EndIf
    
    { Proses Distribusi ke 4 Start Point }
    For i <- 0 To jumlahWafer - 1 Do
        w.batch <- currentBatch
        w.nomorUrut <- i + 1
        w.startPoint <- (i Mod 4) + 1
        w.labelAwal <- GenerateLabelAwal(w.batch, w.nomorUrut, w.startPoint)
        InsertIntoArray(daftarWafer, w)
    EndFor
    
    { Memanggil fungsi Selection Sort untuk mengurutkan tampilan label }
    Call selectionSortLabel(daftarWafer)
EndProcedure