import matplotlib.pyplot as plt
import colormaps as cmaps
import numpy as np

def simpleaxis(ax):
    """
    Removes axis lines
    """
    # plt.axis('off')
    ax.set_axis_off()
    # ax.spines['top'].set_visible(False)
    # ax.get_xaxis().tick_bottom()


def raster(spike_times,
           ax = None,
           figsize = (12,8),
           ylabel = "Cell Id",
           title = None,
           color="k"):
    """
    Raster plot
    """

    if not isinstance(spike_times, list):
        spike_times  = [spike_times]

    num_cells = len(spike_times)
    if ax ==None:
        f = plt.figure(figsize=figsize)
        ax = f.add_subplot(111)

    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.get_xaxis().tick_bottom()
    ax.get_yaxis().tick_left()
    ax.tick_params(axis='x', labelsize=12)

    yticks = []
    yticks.append("")
    for i in range(num_cells):
        for t in spike_times[i][:]:
            ax.vlines(t, i + .9, i + 1.1, color=color)

    plt.ylim(0.8, num_cells+0.2)
    plt.xlabel('t [s]', fontsize = 16)
    plt.ylabel(ylabel, fontsize = 16)
    plt.yticks(range(1,num_cells+1), fontsize = 12)

    if not title==None:
        ax.set_title(title)
    plt.tight_layout()
    return ax


def animateImshowPlots(data,
                       dt = None,
                       figsize = (8,15),
                       cmap = cmaps.inferno,
                       colorbar = False,
                       remove_axes = True,
                       save_animation = False,
                       animation_name = "unnamed" ):

    """
    Animate imshow plots
    """
    import matplotlib.animation as animation
    from mpl_toolkits.mplot3d import axes3d
    from mpl_toolkits.axes_grid1 import make_axes_locatable
    import time

    num_subplots = len(data)
    imshowPlots = []
    nStates = len(data[0]["t_points"])
    dt = 1 if dt==None else data[0]["t_points"][1]-data[0]["t_points"][0]

    num_cols = 2 if num_subplots >= 2 else num_subplots
    num_rows = int(np.ceil((num_subplots-1)/2.))+1

    fig = plt.figure(figsize=figsize)

    def init():
        for i in range(num_subplots):
            imshowPlots[i].set_data(data[i]["value"][0,:,:])
        ttl.set_text("")
        return imshowPlots, ttl

    def animate(j):
        for i in range(num_subplots):
            imshowPlots[i].set_data(data[i]["value"][j,:,:])
            # imshowPlots[i].autoscale()
        t = j*dt
        ttl.set_text("Time = " + str('%.2f' % (t,)) + "ms")
        return imshowPlots, ttl



    ttl = plt.suptitle("",fontsize = 16)
    extent=[data[0]["spatial_vec"][0],data[0]["spatial_vec"][-1],
            data[0]["spatial_vec"][0],data[0]["spatial_vec"][-1]]
    # ttl = plt.suptitle("",fontsize = 16, x = 0.45, horizontalalignment = "left")

    #Plot stimulus
    colspanStim = 2
    ax = plt.subplot2grid((num_rows, num_cols),(0, 0), colspan = colspanStim)
    if remove_axes: simpleaxis(ax)
    ax.set_title(data[0]["type"])
    ax.set_xlabel(r"$x(\theta)$")
    ax.set_ylabel(r"$y(\theta)$")
    imshowPlots.append(ax.imshow(data[0]["value"][0,:,:],
    origin = "lower",cmap="gray", interpolation="none", extent = extent))
    if(colorbar):
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="5%", pad=0.05)
        plt.colorbar(imshowPlots[-1], ax=ax, orientation='vertical', cax = cax)

    k = 1
    for i in range(1, num_rows):
        for j in range(num_cols):
            if(k>= num_subplots):
                break
            ax = plt.subplot2grid((num_rows, num_cols),(i, j))
            if remove_axes: simpleaxis(ax)
            ax.set_title(data[k]["type"])
            ax.set_xlabel(r"$x(\theta)$")
            ax.set_ylabel(r"$y(\theta)$")
            imshowPlots.append(ax.imshow(data[k]["value"][0,:,:], cmap=cmap,
                               interpolation="none",
                               origin = "lower",
                               extent = extent,
                               vmin = (data[k]["value"]).min(),
                               vmax = (data[k]["value"]).max()))

            if(colorbar):
                divider = make_axes_locatable(ax)
                cax = divider.append_axes("right", size="5%", pad=0.05)
                plt.colorbar(imshowPlots[-1], ax=ax, orientation='vertical',cax = cax)


            k+=1

    plt.tight_layout()
    plt.subplots_adjust(top=0.95)

    anim = animation.FuncAnimation(fig, animate, init_func=init,
                                   frames=nStates, interval=20, blit=False)

    if(save_animation):
        anim.save(animation_name + ".mp4",fps=30,  writer="avconv", codec="libx264")

    plt.show()

    return anim



def imshowPlotsOfImpulseResponses(data,
                                  x_imshow=True,
                                  y_imshow=True,
                                  idx=0,
                                  idy=0,
                                  figsize=(14,8),
                                  cmap=cmaps.viridis,
                                  colorbar=True,
                                  save_figure=False,
                                  figure_name="unnamed"):

    """
    Imshow plots of impulse response functions
    """
    num_cols = len(data)
    num_rows = int(x_imshow) + int(y_imshow)

    fig, axarr = plt.subplots(num_rows, num_cols, figsize=figsize, sharey=True)

    if(num_rows == 1):
        axarr =  np.array(axarr).reshape(1,num_cols)
    if(num_cols ==1):
        axarr =  np.array(axarr).reshape(num_rows,1)
    levels = np.arange(-5., 5., 0.23)
    for j in range(num_cols):
        axarr[0,j].set_title(data[j]["type"])
        i=0
        if(x_imshow):
            axarr[i,j].set_adjustable('box-forced')
            extent=[data[j]["spatial_vec"][0],data[j]["spatial_vec"][-1],
                    data[j]["t_points"][0],data[j]["t_points"][-1]]
            im = axarr[i,j].imshow(data[j]["value"][:,idy,:], extent= extent,
                                   cmap=cmap, origin="lower", aspect="auto",
                                   interpolation="gaussian")
            axarr[i,j].contour(data[j]["value"][:,idy,:],  levels,
            colors='w',linewidths=0.4,
            extent= extent, aspect="auto")
            axarr[i,j].set_xlabel(r"$x(\theta)$")
            axarr[i,0].set_ylabel(r"$\tau(ms)$")

            if(colorbar):
                fig.colorbar(im, ax=axarr[i,j],orientation='horizontal')
            i+=1
        if(y_imshow):
            axarr[i,j].set_adjustable('box-forced')
            extent=[data[j]["spatial_vec"][0],data[j]["spatial_vec"][-1],
                    data[j]["t_points"][0],data[j]["t_points"][-1]]
            im = axarr[i,j].imshow(data[j]["value"][:,:,idx], extent= extent,
                                   cmap=cmap, origin="lower",aspect="auto",
                                   interpolation="gaussian")

            axarr[i,j].contour(data[j]["value"][:,:,idx],  levels,
            colors='w',linewidths=0.4,
            extent= extent,  aspect="auto")
            axarr[i,j].set_xlabel(r"$y(\theta)$")
            axarr[i,0].set_ylabel(r"$\tau(ms)$")

            if(colorbar):
                fig.colorbar(im, ax=axarr[i,j], orientation='horizontal')
            i+=1
    if(save_figure):
        fig.save(animation_name + ".svg")
    plt.show()






def plot3dOfImpulseResponses(data,
                             x_3d=True,
                             y_3d=True,
                             num_skip = 2,
                             idx=0,
                             idy=0,
                             figsize=(15,10),
                             cmap=cmaps.inferno,
                             colorbar=False,
                             save_figure=False,
                             figure_name="unnamed"):

    """
    3D plots of impulse response functions
    """
    from mpl_toolkits.mplot3d import Axes3D
    num_cols = len(data)
    num_rows = int(x_3d) + int(y_3d)


    fig = plt.figure(figsize=figsize)
    for j in range(num_cols):
        S = data[j]["spatial_vec"]
        T = data[j]["t_points"]
        T, S = np.meshgrid(S, T)
        i=0
        if(x_3d):
            ax = plt.subplot2grid((num_rows, num_cols),(i,j), projection='3d')
            surf = ax.plot_surface(S[::num_skip,::num_skip],
                                   T[::num_skip,::num_skip],
                                   data[j]["value"][::num_skip, idy, ::num_skip],
                                   cmap=cmap, edgecolors="k", alpha=0.9, shade=False,
                                   vmin = (data[j]["value"]).min(),
                                   vmax = (data[j]["value"]).max(),
                                   rstride=1, cstride=1, linewidth=0.0, antialiased=False)
            ax.set_title(data[j]["type"])
            ax.set_ylabel(r"$x(\theta)$")
            ax.set_xlabel(r"$\tau(ms)$")
            ax.set_zlabel(r"$W$")
            ax.set_zlim3d(np.min(data[j]["value"][::num_skip, idy, ::num_skip]),
                          np.max(data[j]["value"][::num_skip, idy, ::num_skip]))
            ax.view_init(elev=46., azim=130)
            if(colorbar):
                cbar = plt.colorbar(surf, ax=ax, orientation='horizontal')
            i+=1
        if(y_3d):
            ax = plt.subplot2grid((num_rows, num_cols),(i,j), projection='3d')
            surf = ax.plot_surface(S[::num_skip,::num_skip],
                                   T[::num_skip,::num_skip],
                                   data[j]["value"][::num_skip, ::num_skip, idx],
                                   cmap=cmap, edgecolors="k", alpha=0.9, shade=False,
                                   vmin = (data[j]["value"]).min(),
                                   vmax = (data[j]["value"]).max(),
                                   rstride=1, cstride=1, linewidth=0.0, antialiased=False)
            ax.set_title(data[j]["type"])
            ax.set_ylabel(r"$y(\theta)$")
            ax.set_xlabel(r"$\tau(ms)$")
            ax.set_zlabel(r"$W$")
            ax.set_zlim3d(np.min(data[j]["value"][::num_skip, ::num_skip, idx]),
                          np.max(data[j]["value"][::num_skip, ::num_skip, idx]))
            ax.view_init(elev=46., azim=130)
            if(colorbar):
                fig.colorbar(surf, ax=ax, orientation='horizontal')
            i+=1


    fig.tight_layout()
    if(save_figure):
        fig.save(animation_name + ".svg")
    plt.show()




def line3dPlotsOfImpulseResponses(data,
                                  x_line3d=True,
                                  y_line3d=True,
                                  num_skip = 10,
                                  idx=64,
                                  idy=64,
                                  figsize = (14,8),
                                  cmap = cmaps.inferno,
                                  colorbar = False,
                                  save_figure = False,
                                  figure_name = "unnamed"):

    """
    3D line plot of impulse response functions
    """
    from mpl_toolkits.mplot3d import Axes3D
    num_cols = len(data)
    num_rows = int(x_line3d) + int(y_line3d)

    fig = plt.figure(figsize=figsize)
    for j in range(num_cols):
        ids = range(0, len(data[j]["spatial_vec"]), num_skip)
        i=0
        if(x_line3d):
            ax = plt.subplot2grid((num_rows, num_cols),(i,j), projection='3d')
            for x in ids:
                ax.plot(data[j]["t_points"],
                data[j]["spatial_vec"][x]*np.ones(len(data[j]["t_points"])),
                data[j]["value"][:,idy, x],
                "-b", linewidth=1.0)

            ax.set_title(data[j]["type"])
            ax.set_xlabel(r"$\tau(ms)$")
            ax.set_ylabel(r"$x(\theta)$")
            ax.set_zlabel(r"$W(x, y_a, \tau)$")
            # ax.view_init(elev=17., azim=128)
            ax.set_xlim3d(data[j]["t_points"][0], data[j]["t_points"][-1])
            ax.set_ylim3d(data[j]["spatial_vec"][0], data[j]["spatial_vec"][-1])
            ax.set_zlim3d(np.min(data[j]["value"][:,idy, ids]),
                          np.max(data[j]["value"][:,idy, ids]))
            i+=1
        if(y_line3d):
            ax = plt.subplot2grid((num_rows, num_cols),(i,j), projection='3d')
            for y in ids:
                ax.plot(data[j]["t_points"],
                data[j]["spatial_vec"][y]*np.ones(len(data[j]["t_points"])),
                data[j]["value"][:,y, idx],
                "-b", linewidth=1.0)

            ax.set_title(data[j]["type"])
            ax.set_xlabel(r"$\tau(ms)$")
            ax.set_ylabel(r"$y(\theta)$")
            ax.set_zlabel(r"$W(x_a, y, \tau)$")
            # ax.view_init(elev=17., azim=128)
            ax.set_xlim3d(data[j]["t_points"][0], data[j]["t_points"][-1])
            ax.set_ylim3d(data[j]["spatial_vec"][0], data[j]["spatial_vec"][-1])
            ax.set_zlim3d(np.min(data[j]["value"][:,ids, idx]),
                          np.max(data[j]["value"][:,ids, idx]))

            i+=1
    plt.tight_layout()
    if(save_figure):
        fig.save(animation_name + ".svg")
    plt.show()



if __name__ == "__main__":
    import h5py
    from glob import glob
    import Simulation as sim
    plt.close("all")

    # outputFile =  "/home/milad/Dropbox/projects/lgn/code/lgn-simulator/apps/spatialSummation/spatialSummation.h5"
    outputFile =  "/media/milad/scratch/lgn-simulator/simulations/spatialSummation/20160506-114119/20160506-114119.h5"

    # outputFile = glob(outputFilePath)[0]
    # print outputFile
    f = h5py.File(outputFile, "r")
    exp = sim.Simulation(None, f)

    Ns = exp.integrator.Ns
    Nt = exp.integrator.Nt

    # S = {"type" : "Stimulus",
    #         "value" : exp.stimulus.spatio_temporal,
    #         "t_points" : exp.integrator.t_points,
    #         "spatial_vec" : exp.integrator.s_points
    #         }

    #
    # Wg = {"type" : "Ganglion",
    #             "value" : exp.ganglion.impulse_response["spatio_temporal"],
    #             "t_points" : exp.integrator.t_points,
    #             "spatial_vec" : exp.integrator.s_points
    #             }
    # Wr = {"type" : "Relay",
    #          "value" : exp.relay.impulse_response["spatio_temporal"],
    #          "t_points" : exp.integrator.t_points,
    #          "spatial_vec" : exp.integrator.s_points
    #         }
    #
    # Wi = {"type" : "Interneuron",
    #              "value" : exp.interneuron.impulse_response["spatio_temporal"],
    #              "t_points" : exp.integrator.t_points,
    #              "spatial_vec" : exp.integrator.s_points
    #             }
    #
    # Wc = {"type" : "Cortical",
    #              "value" : exp.cortical.impulse_response["spatio_temporal"],
    #              "t_points" : exp.integrator.t_points,
    #              "spatial_vec" : exp.integrator.s_points
    #             }

    Rg = {"type" : "Ganglion",
                "value" : exp.ganglion.response["spatio_temporal"],
                "t_points" : exp.integrator.t_points,
                "spatial_vec" : exp.integrator.s_points
                }
    Rr = {"type" : "Relay",
             "value" : exp.relay.response["spatio_temporal"],
             "t_points" : exp.integrator.t_points,
             "spatial_vec" : exp.integrator.s_points
            }

    Ri = {"type" : "Interneuron",
             "value" : exp.interneuron.response["spatio_temporal"],
             "t_points" : exp.integrator.t_points,
             "spatial_vec" : exp.integrator.s_points
            }
    Rc = {"type" : "Cortical",
                 "value" : exp.cortical.response["spatio_temporal"],
                 "t_points" : exp.integrator.t_points,
                 "spatial_vec" : exp.integrator.s_points
                }

    # data = [Wg,Wr,Wi,Wc]
    # line3dPlotsOfImpulseResponses(data, idx=Ns/2, idy=Ns/2, num_skip=1,y_line3d=True)
    # plot3dOfImpulseResponses(data[:], colorbar=True, y_3d=True,num_skip=6, idx=Ns/2, idy=Ns/2)
    # imshowPlotsOfImpulseResponses(data, idx=Ns/2, idy=Ns/2,y_imshow=True)
    # data = [ S, Wg, Rg, Wr, Rr, Wi, Ri,  Wc, Rc]
    data = [ Rg, Rr, Ri, Rc]
    plt.figure()
    plt.plot(exp.integrator.t_points, exp.ganglion.response["spatio_temporal"][:, Ns/2,Ns/2], label = "G")
    plt.plot(exp.integrator.t_points, exp.relay.response["spatio_temporal"][:, Ns/2,Ns/2], label = "R")
    plt.plot(exp.integrator.t_points, exp.interneuron.response["spatio_temporal"][:, Ns/2,Ns/2], label = "I")
    plt.plot(exp.integrator.t_points, exp.cortical.response["spatio_temporal"][:, Ns/2,Ns/2], label = "C")
    plt.legend()
    animateImshowPlots(data, exp.integrator.dt,
    colorbar = True, remove_axes = False,
    save_animation = False, animation_name = "newTest")

    plt.show()







        # plt.figure()
        # plt.imshow(exp.ganglion.response["spatioTemporal"][0,:,:] )
        # plt.colorbar()

        # delta = 20
        # points = Nt-delta
        #
        # plt.figure()
        # plt.imshow(exp.ganglion.response["spatioTemporal"][0,:,:] - exp.stimulus.spatioTemporal[Nt-delta])
        # plt.colorbar()
        # plt.figure()
        #
        #
        # #
        # res = exp.singleCellTemporalResponse("ganglion", Ns/2, Ns/2)[:]
        # stim= exp.stimulus.spatioTemporal[:, Ns/2, Ns/2][::-1][:Nt-delta]
        # stim_real= exp.stimulus.spatioTemporal[:, Ns/2, Ns/2]
        #
        # plt.plot(exp.integrator.timeVec[:], res, '--r', label="res")
        # plt.plot(exp.integrator.timeVec[:Nt-delta], stim, '--b', label="stim")
        # plt.plot(exp.integrator.timeVec, stim_real, '--g', label="stim_real")
        # plt.xlim([0,1200])
        # plt.legend(loc="center right")
        # print abs(stim[:points]-res[:points]).max()
