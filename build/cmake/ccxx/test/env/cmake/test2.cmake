
#aux_source_directory([=[C:\ddd\ygct\ygct_igs_cc4000\cc4000\src\Docker\libsvncpp]=] gs_sources)
aux_source_directory(. gs_sources)
foreach (sSource ${gs_sources})
    message(${sSource})
endforeach ()