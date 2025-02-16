package com.taotao.jvm.hotspot.src.share.vm.oops;

import lombok.Data;

import java.util.ArrayList;
import java.util.List;

@Data
public class BootstrapMethods extends AttributeInfo {

    private String attrName;

    private int numBootstrapMethods;
    private List<Item> bootstrapMethods = new ArrayList<>();

    @Data
    public class Item {
        private int bootstrapMethodRef;
        private int numBootstrapArguments;
        private int[] bootstrapArguments;

        public void initContainter() {
            bootstrapArguments = new int[numBootstrapArguments];
        }
    }

}
