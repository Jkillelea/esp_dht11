using Plots
plotly()
p = []

const SECOND   = 1
const MINUTE   = 60*SECOND
const HOUR     = 60*MINUTE
const RANGEMAX = 30*MINUTE
const SPREAD   = 5

for file in ARGS # each file
    data = readdlm(file)
    ids = unique(data[:, 2])

    for id in ids # each unique id in file
        idx  = data[:, 2] .== id
        t = data[idx, 1]
        dt = t - t[1]
        vals = data[idx, 3]

        smoothed_data = zeros(length(vals), 1)
        for i in 1:length(vals)
            range = (i-SPREAD):(i+SPREAD)
            try
                smoothed_data[i] = mean(vals[range])
            catch # index by range fails
                smoothed_data[i] = vals[i]
            end
        end
        
        # xlabel = "seconds elapsed"
        dt /= 60 # convert to minutes
        xlabel = "minutes elapsed"
        # dt /= 60*60 # convert to hours
        # xlabel = "hours elapsed"

        p = plot!(dt, smoothed_data,
                 size = [1600, 900],
                 width = 3,
                 label = "$id | $file",
                 xlabel = xlabel
                 )
    end
end

gui(p)
